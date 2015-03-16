//
//  Renderable.cpp
//  CGP-Ex1
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

Renderable::Renderable() :
_vao(0), _vbo(0), _ibo(0), _world(1.0f)
{
    std::cout << "In Renderable()" << std::endl;
    init();
}

Renderable::~Renderable()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
}

void Renderable::init()
{
    programManager::sharedInstance()
    .createProgram("default",
                   SHADERS_DIR "SimpleShader.vert",
                   SHADERS_DIR "SimpleShader.frag");
    
    GLuint program = programManager::sharedInstance().programWithID("default");
    
    // Obtain uniform variable handles:
    _fillColorUV  = glGetUniformLocation(program, "fillColor");
    _gpuWVP  = glGetUniformLocation(program, "wvp");
    
    // Initialize vertices buffer and transfer it to OpenGL
    {
        // Create and bind the object's Vertex Array Object:
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        
        // Create and vertex data and index data Vertex Buffer Objects:
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ibo);
                
        // Tell OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
        // Obtain attribute handles:
        _posAttrib = glGetAttribLocation(program, "position");
        glEnableVertexAttribArray(_posAttrib);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(_posAttrib, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);
        
        // Unbind vertex array:
        glBindVertexArray(0);
    }
}

void Renderable::draw()
{
    // Set the program to be used in subsequent lines:
    glUseProgram(programManager::sharedInstance().programWithID("default"));
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Also try using GL_FILL and GL_POINT
    
    mat4 wvp = Camera::Instance().getViewProjection() * _world;
    
    // Pass the uniform variables
    {
        // wvp matrix
        glUniformMatrix4fv (_gpuWVP, 1, GL_FALSE, value_ptr(wvp));
        
        // Terrain color
        glUniform4f(_fillColorUV, 0.3f, 0.5f, 0.3f, 1.0);
    }
    
    // Draw using the state stored in the Vertex Array object:
    {
        glBindVertexArray(_vao);
        
        glDrawElements(GL_TRIANGLES, _nElements, GL_UNSIGNED_SHORT, (GLvoid*)0);
        
        // Unbind the Vertex Array object
        glBindVertexArray(0);
    }
    
    // Cleanup, not strictly necessary
    glUseProgram(0);
}