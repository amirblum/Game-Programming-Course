//
//  Corridor.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Corridor.h"
#include "Camera.h"

#include <iostream>

/**
 * Corridor constructor
 */
Corridor::Corridor(float width, float height, float length) :
_width(width),
_height(height),
_length(length)
{
    // Initialize corridor
    static const GLfloat vertices[] = {
        // Left wall
        -width/2, 0.0f, 0.0f, 1.0f,
        -width/2, height, 0.0f, 1.0f,
        -width/2, 0.0f, -length, 1.0f,
        -width/2, height, -length, 1.0f,
        // Right wall
        width/2, 0.0f, 0.0f, 1.0f,
        width/2, height, 0.0f, 1.0f,
        width/2, 0.0f, -length, 1.0f,
        width/2, height, -length, 1.0f
    };

    
    // Push VBO
//    std::cout << "Pushing VBO" << std::endl;
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    static const GLubyte indices[] = {
        0, 1, 2,
        1, 3, 2,
        1, 5, 3,
        3, 7, 5,
        5, 4, 7,
        4, 6, 7,
        4, 0, 6,
        0, 2, 6
    };
    
    _nElementIndices = sizeof(indices) / sizeof(GLubyte);
    
    // Push IBO
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

/**
 * Deconstructor
 */
Corridor::~Corridor()
{
}

/**
 * Get width
 */
float Corridor::getWidth()
{
    return _width;
}

/**
 * Get height
 */
float Corridor::getHeight()
{
    return _height;
}

/**
 * Get length
 */
float Corridor::getLength()
{
    return _length;
}