//
//  Renderable.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Renderable.h"
#include "ShaderIO.h"

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"


#define SHADERS_DIR "shaders/"

Renderable::Renderable(std::string shaderProgram,
                       std::string vertexShaderFilename,
                       std::string fragmentShaderFilename,
                       vec3 positionVec, vec3 scaleVec) :
_vao(0), _vbo(0), _ibo(0), _world(1.0f), _bumpMappingOn(false)
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
        //        std::cout << "Generating VertexAttribute Pointer" << std::endl;
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
    
    // Initialize texture/bump
    {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &_tex);
        
        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &_bump);
    }
    
    // Obtain uniform variable handles:
    {
        _worldUniform  = glGetUniformLocation(_shaderProgram, "W");
        _viewProjectionUniform  = glGetUniformLocation(_shaderProgram, "PV");
        _textureUniform  = glGetUniformLocation(_shaderProgram, "textureSampler");
        _bumpUniform  = glGetUniformLocation(_shaderProgram, "bumpSampler");
        _bumpMappingOnUniform = glGetUniformLocation(_shaderProgram, "bumpMappingOn");
    }
    
    // Set up world translation
    {
        _world = _world * translate(mat4(1.0f), positionVec);
        _world = _world * scale(mat4(1.0f), scaleVec);
    }
}

Renderable::~Renderable()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
}

/**
 * Tell OpenGL to draw
 */
void Renderable::draw()
{
    // Set the program to be used in subsequent lines:
    glUseProgram(_shaderProgram);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_FILL and GL_POINT
    
    // Pass the uniform variables
    {
        // w,vp matrices
        glUniformMatrix4fv(_worldUniform, 1, GL_FALSE, value_ptr(_world));
        glUniformMatrix4fv(_viewProjectionUniform, 1, GL_FALSE,
                           value_ptr(Camera::Instance().getViewProjection()));
        
        glUniform1i(_textureUniform, 0);
        glUniform1i(_bumpUniform, 1);
        glUniform1i(_bumpMappingOnUniform, _bumpMappingOn);
    }
    
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

/** Empty customBindings by default */
void Renderable::customBindings()
{
}

mat4 Renderable::getWorldMat()
{
    return _world;
}

void Renderable::toggleBumpMapping()
{
    _bumpMappingOn = !_bumpMappingOn;
}