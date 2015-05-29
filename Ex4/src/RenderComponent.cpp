//
//  RenderComponent.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "RenderComponent.h"
#include "ShaderIO.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include <SOIL2/SOIL2.h>

#define SHADERS_DIR "shaders/"
#define VERTEX_EXT ".vert"
#define FRAGMENT_EXT ".frag"

RenderComponent::RenderComponent(std::string shaderProgram) :
_vao(0), _vbo(0), _ibo(0),
_supportVBOs(),
_textures(), _uniforms(),
_numIndices(0)
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
        
        // Unbind vertex array:
        glBindVertexArray(0);
    }
}

RenderComponent::~RenderComponent()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
    
    glDeleteBuffers(_supportVBOs.size(), &_supportVBOs[0]);
    
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
    for (std::shared_ptr<TextureComponent> texture : _textures) {
        texture->bind();
    }
    
    // Draw using the state stored in the Vertex Array object:
    {
        glBindVertexArray(_vao);
        
        if (numInstances > 1) {
            glDrawElementsInstanced(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, (GLvoid*)0, numInstances);
        } else {
            glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, (GLvoid*)0);
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

void RenderComponent::setPTNVBO(std::vector<Vertex> vertices)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    // Tell OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
    // Obtain position handle:
    GLint positionAttrib = glGetAttribLocation(_shaderProgram, "position");
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, // attribute handle
                          3,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)0);
    // Obtain texcoords handle:
    GLint texcoordsAttrib = glGetAttribLocation(_shaderProgram, "texcoords");
    glEnableVertexAttribArray(texcoordsAttrib);
    glVertexAttribPointer(texcoordsAttrib, // attribute handle
                          2,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)12);
    // Obtain attribute handles:
    GLint normalAttrib = glGetAttribLocation(_shaderProgram, "normal");
    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, // attribute handle
                          3,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)20);
    
    glBindVertexArray(0);
}

void RenderComponent::setPositionsVBO(std::vector<GLfloat> positions)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
    
    // Tell OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
    // Obtain attribute handles:
    GLint posAttrib = glGetAttribLocation(_shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          (GLvoid*)0);
    
    glBindVertexArray(0);
}

void RenderComponent::setIBO(std::vector<GLuint> indices)
{
    glBindVertexArray(_vao);
    
    // Push IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    _numIndices = indices.size();
    
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

void RenderComponent::addTexture(std::shared_ptr<TextureComponent> textureComponent)
{
    _textures.push_back(textureComponent);
}

TextureComponent::TextureComponent(GLenum type, GLuint textureNum) :
_type(type), _textureNum(textureNum)
{
    
}

TextureComponent::~TextureComponent()
{
    glDeleteTextures(1, &_textureNum);
}

std::shared_ptr<TextureComponent> TextureComponent::create2DTexture(std::string filename)
{
    GLuint textureNum = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (textureNum == 0) {
        std::cout << "Error loading texture " << filename << std::endl;
        std::cout << "Reason: " << SOIL_last_result() << std::endl;
        return NULL;
    }
    
    return std::make_shared<TextureComponent>(GL_TEXTURE_2D, textureNum);
}

std::shared_ptr<TextureComponent> TextureComponent::createCubeTexture(std::string cubeLF,
                                                      std::string cubeFT,
                                                      std::string cubeRT,
                                                      std::string cubeBK,
                                                      std::string cubeUP,
                                                      std::string cubeDN)
{
    GLuint textureNum = SOIL_load_OGL_cubemap(cubeRT.c_str(),
                                               cubeLF.c_str(),
                                               cubeUP.c_str(),
                                               cubeDN.c_str(),
                                               cubeFT.c_str(),
                                               cubeBK.c_str(),
                                               SOIL_LOAD_AUTO,
                                               SOIL_CREATE_NEW_ID, 0);
    if (textureNum == 0) {
        std::cout << "Error loading cube texture " << cubeLF << " (and it's other sides)" << std::endl;
        std::cout << "Reason: " << SOIL_last_result() << std::endl;
        return NULL;
    }
    
    return std::make_shared<TextureComponent>(GL_TEXTURE_CUBE_MAP, textureNum);
}

void TextureComponent::bind()
{
    glBindTexture(_type, _textureNum);

}
