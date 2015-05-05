//
//  RenderComponent.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "RenderComponent.h"
#include "ShaderIO.h"

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "SOIL2/SOIL2.h"


#define SHADERS_DIR "shaders/"
#define VERTEX_EXT ".vert"
#define FRAGMENT_EXT ".frag"

RenderComponent::RenderComponent(std::string shaderProgram) :
_vao(0), _vbo(0), _ibo(0),
_supportVBOs(),
_textureCount(0), _textures(), _uniforms(),
_nElementIndices(0)
{
    programManager::sharedInstance()
    .createProgram(shaderProgram,
                   (SHADERS_DIR + shaderProgram + VERTEX_EXT).c_str(),
                   (SHADERS_DIR + shaderProgram + FRAGMENT_EXT).c_str());
    
    _shaderProgram = programManager::sharedInstance().programWithID(shaderProgram);
    
    // Initialize VBO and transfer it to OpenGL
    {
        // Create and bind the object's Vertex Array Object:
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        
        // Create and vertex data and index data Vertex Buffer Objects:
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ibo);
        
        // Tell OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
        // Obtain attribute handles:
        GLint posAttrib = glGetAttribLocation(_shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(posAttrib, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              (GLvoid*)0);
        
        // Unbind vertex array:
        glBindVertexArray(0);
    }
}

RenderComponent::~RenderComponent()
{
    std::cout << "Deconstructing render component" << std::endl;
    
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
    
    for (TextureComponent *texture : _textures) {
        delete texture;
    }
    
    for (auto kv : _uniforms) {
        delete kv.second;
    }
}

/**
 * Render the object. Default to instanced = false
 */
void RenderComponent::render(mat4 worldTransform)
{
    render(worldTransform, 1);
}

/**
 * Tell OpenGL to draw
 */
void RenderComponent::render(mat4 worldTransform, int numInstances)
{
    // Set the program to be used in subsequent lines:
    glUseProgram(_shaderProgram);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_LINE and GL_POINT
    
    // Update world/viewProjection uniforms
    setUniform<mat4, UNIFORM_MAT4>("W", worldTransform);
    setUniform<mat4, UNIFORM_MAT4>("PV", Camera::MainCamera()->getViewProjection());
    
    // Pass the uniform variables
    for (auto kv : _uniforms) {
        sendUniform(kv.second);
    }
    
    // Bind/pass textures
    for (auto texture : _textures) {
        sendTexture(texture);
    }
    
    // Draw using the state stored in the Vertex Array object:
    {
        glBindVertexArray(_vao);
        
        if (numInstances > 1) {
            glDrawElementsInstanced(GL_TRIANGLES, _nElementIndices, GL_UNSIGNED_BYTE, (GLvoid*)0, numInstances);
        } else {
            glDrawElements(GL_TRIANGLES, _nElementIndices, GL_UNSIGNED_BYTE, (GLvoid*)0);
        }
        
        // Unbind the Vertex Array object
        glBindVertexArray(0);
    }
    
    // Cleanup
    glUseProgram(0);
}

void RenderComponent::sendUniform(UniformVariable *uniform)
{
    switch (uniform->getType()) {
        case UNIFORM_FLOAT:
        {
            UniformVariableFloat *uniformf = dynamic_cast<UniformVariableFloat*>(uniform);
            glUniform1f(uniformf->handle, uniformf->value);
            break;
        }
        case UNIFORM_INT:
        {
            UniformVariableInt *uniformi = dynamic_cast<UniformVariableInt*>(uniform);
            glUniform1i(uniformi->handle, uniformi->value);
            break;
        }
        case UNIFORM_BOOLEAN:
        {
            UniformVariableBool *uniformb = dynamic_cast<UniformVariableBool*>(uniform);
            glUniform1i(uniformb->handle, uniformb->value);
            break;
        }
        case UNIFORM_VEC3:
        {
            UniformVariableVec3 *uniformv3 = dynamic_cast<UniformVariableVec3*>(uniform);
            glUniform3fv(uniformv3->handle, 1, value_ptr(uniformv3->value));
            break;
        }
        case UNIFORM_VEC4:
        {
            UniformVariableVec4 *uniformv4 = dynamic_cast<UniformVariableVec4*>(uniform);
            glUniform3fv(uniformv4->handle, 1, value_ptr(uniformv4->value));
            break;
        }
        case UNIFORM_MAT3:
        {
            UniformVariableMat3 *uniformm3 = dynamic_cast<UniformVariableMat3*>(uniform);
            glUniformMatrix3fv(uniformm3->handle, 1, GL_FALSE, value_ptr(uniformm3->value));
            break;
        }
        case UNIFORM_MAT4:
        {
            UniformVariableMat4 *uniformm4 = dynamic_cast<UniformVariableMat4*>(uniform);
            glUniformMatrix4fv(uniformm4->handle, 1, GL_FALSE, value_ptr(uniformm4->value));
            break;
        }
        
        default:
            break;
    }
}

void RenderComponent::sendTexture(TextureComponent *textureComponent)
{
    glUniform1i(textureComponent->samplerUniform, textureComponent->textureNumber);
    glActiveTexture(textureComponent->textureNumber);
    glBindTexture(textureComponent->type, textureComponent->texture);
}

void RenderComponent::setVBO(std::vector<vec4> vertices)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec4), &vertices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void RenderComponent::setVBO(std::vector<GLfloat> vertices)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void RenderComponent::setIBO(std::vector<GLubyte> indices)
{
    glBindVertexArray(_vao);
    
    // Push IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    _nElementIndices = indices.size();
    
    glBindVertexArray(0);
}

GLuint RenderComponent::createSupportVBO(GLenum type, int size, std::string name, int divisor)
{
    glBindVertexArray(_vao);
    GLint attribLocation = glGetAttribLocation(_shaderProgram, name.c_str());
    
    GLuint supportVBO;
    glGenBuffers(1, &supportVBO);
    glBindBuffer(GL_ARRAY_BUFFER, supportVBO);
    glEnableVertexAttribArray(attribLocation);
    glVertexAttribPointer(attribLocation, // attribute handle
                          size,          // number of scalars per vertex
                          type,         // scalar type
                          GL_FALSE,
                          0,
                          (GLvoid*)0);
    glVertexAttribDivisor(attribLocation, divisor);
    
    glBindVertexArray(0);
    
    _supportVBOs.push_back(supportVBO);
    return supportVBO;
}

void RenderComponent::addTexture(std::string textureFile, GLenum textureType)
{
    if (_textureCount >= 32) {
        std::cout << "ERROR: Cannot bind more than 32 textures at a time" << std::endl;
        return;
    }
    
    TextureComponent *newTexture = new TextureComponent();
    
//    BImage textureMap;
//    if (!textureMap.readImage(textureFile.c_str())) {
//        std::cout << "Failed to read texture: " << textureFile << std::endl;
//        exit(1);
//    }
    
    
//    int imageWidth, imageHeight, imageBits;
//    unsigned char *imageData = stbi_load(textureFile.c_str(), &imageWidth, &imageHeight, &imageBits, 0);
    
    // Get the texture number. This is implementation specific and is BAD. Whatever.
    newTexture->textureNumber = GL_TEXTURE0 + _textureCount++;
    glActiveTexture(newTexture->textureNumber);
    
    newTexture->texture = SOIL_load_OGL_texture(textureFile.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    
    char sampleName[17];
    sprintf(sampleName, "textureSampler%u", _textureCount);
    newTexture->samplerUniform = glGetUniformLocation(_shaderProgram, sampleName);
    
    _textures.push_back(newTexture);
    
//    // Get the texture number. This is implementation specific and is BAD. Whatever.
//    newTexture->textureNumber = GL_TEXTURE0 + _textureCount++;
//    char sampleName[17];
//    sprintf(sampleName, "textureSampler%u", _textureCount);
//    newTexture->samplerUniform = glGetUniformLocation(_shaderProgram, sampleName);
//    // "Bind" the newly created texture : all future texture functions will modify this texture
//    glActiveTexture(newTexture->textureNumber);
//    glGenTextures(1, &(newTexture->texture));
//    glBindTexture(GL_TEXTURE_2D, newTexture->texture);
//    
//    // Give the image to OpenGL
//    glTexImage2D(textureType, 0,
//                 GL_RGBA,
//                 imageWidth, imageHeight,
//                 0,
//                 GL_RGB, GL_UNSIGNED_BYTE,
//                 imageData);
//    
//    // Format texture
//    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    
//    _textures.push_back(newTexture);
//    
//    glBindTexture(textureType, 0);
}