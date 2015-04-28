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
#include "bimage.h"


#define SHADERS_DIR "shaders/"

RenderComponent::RenderComponent(std::string shaderProgram,
                       std::string vertexShaderFilename,
                       std::string fragmentShaderFilename) :
_vao(0), _vbo(0), _ibo(0), _textureCount(0), _textures(), _uniforms()
{
    programManager::sharedInstance()
    .createProgram(shaderProgram,
                   (SHADERS_DIR + vertexShaderFilename).c_str(),
                   (SHADERS_DIR + fragmentShaderFilename).c_str());
    
    _shaderProgram = programManager::sharedInstance().programWithID(shaderProgram);
    
    // Initialize VBO and transfer it to OpenGL
    {
        // Create and bind the object's Vertex Array Object:
        //        std::cout << "Generating VBO" << std::endl;
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        
        // Create and vertex data and index data Vertex Buffer Objects:
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ibo);
        
        // Tell OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
        // Obtain attribute handles:
        _posAttrib = glGetAttribLocation(_shaderProgram, "position");
        glEnableVertexAttribArray(_posAttrib);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(_posAttrib, // attribute handle
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
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
    
    for (TextureComponent *texture : _textures) {
        delete texture;
    }
    
    for (auto kv : _uniforms) {
        delete kv->second;
    }
}

/**
 * Tell OpenGL to draw
 */
void RenderComponent::render(mat4 worldTransform)
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
//    {
//        // w,vp matrices
//        glUniformMatrix4fv(_worldUniform, 1, GL_FALSE, value_ptr(worldTransform));
//        glUniformMatrix4fv(_viewProjectionUniform, 1, GL_FALSE,
//                           value_ptr(Camera::MainCamera()->getViewProjection()));
//        
//        glUniform1i(_textureUniform, 0);
//        glUniform1i(_bumpUniform, 1);
//    }
    
    // Draw using the state stored in the Vertex Array object:
    {
        customBindings();
        
        glBindVertexArray(_vao);
        
        glDrawElements(GL_TRIANGLES, _nElementIndices, GL_UNSIGNED_BYTE, (GLvoid*)0);
        
        // Unbind the Vertex Array object
        glBindVertexArray(0);
    }
    
    // Cleanup, not strictly necessary
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

/** Empty customBindings by default */
void RenderComponent::customBindings()
{
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
    
    // Push IBO
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    _nElementIndices = indices.size();
}

void RenderComponent::addTexture(std::string textureFile, GLenum textureType)
{
    if (_textureCount >= 32) {
        std::cout << "ERROR: Cannot bind more than 32 textures at a time" << std::endl;
        return;
    }
    
    TextureComponent *newTexture = new TextureComponent();
    
    BImage textureMap;
    if (!textureMap.readImage(textureFile.c_str())) {
        std::cout << "Failed to read texture: " << textureFile << std::endl;
        exit(1);
    }
    
    // Get the texture number. This is implementation specific and is BAD. Whatever.
    newTexture->textureNumber = GL_TEXTURE0 + _textureCount++;
    char sampleName[17];
    sprintf(sampleName, "textureSampler%u", _textureCount);
    newTexture->samplerUniform = glGetUniformLocation(_shaderProgram, sampleName);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glActiveTexture(newTexture->textureNumber);
    glGenTextures(1, &(newTexture->texture));
    glBindTexture(GL_TEXTURE_2D, newTexture->texture);
    
    // Give the image to OpenGL
    glTexImage2D(textureType, 0,
                 GL_RGBA,
                 textureMap.width(), textureMap.height(),
                 0,
                 GL_RGB, GL_UNSIGNED_BYTE,
                 textureMap.getImageData());
    
    // Format texture
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    _textures.push_back(newTexture);
    
    glBindTexture(textureType, 0);
}

template <class T, UniformType type>
void RenderComponent::setUniform(std::string name, T value)
{
    UniformVariableDerived<T, type> *uniform;
    auto uniformIterator = _uniforms.find(name);
    if (uniformIterator == _uniforms.end()) {
        // Create the uniform the first time
        uniform = new UniformVariableDerived<T, type>();
        uniform->handle = glGetUniformLocation(_shaderProgram, name.c_str());
        _uniforms[name] = uniform;
    } else {
        uniform = dynamic_cast<UniformVariableDerived<T, type>*>(uniformIterator->second);
    }
    
    uniform->value = value;
}