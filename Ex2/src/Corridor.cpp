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
//#include "stb_image.h"
#include "bimage.h"

static const std::string WALL_TEXTURE = "/Users/amirblum/Development/Game-Programming-Course/Ex2/assets/brickwork-texture.bmp";

/**
 * Corridor constructor
 */
Corridor::Corridor(float width, float height, float length) :
Renderable("corridor", "CorridorShader.vert", "CorridorShader.frag"),
_width(width),
_height(height),
_length(length),
_offset(0)
{
    // Initialize corridor
    
    _world = _world * translate(mat4(1.0f), vec3(0.0f, height/2, -length/2));
    _world = _world * scale(mat4(1.0f), vec3(width, height, length));
    
    static const GLfloat vertices[] = {
        // Left wall
        -0.5f, -0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f,
        
        -0.5f, -0.5f, -0.5f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f,
        
        // Right wall
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f
    };


    // Push VBO
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
    
    // Create corridor texture
    {
        // Load wall image
//        int wallImageWidth, wallImageHeight, wallImageChannels;
//        unsigned char *wallImageData = stbi_load(WALL_TEXTURE.c_str(), &wallImageWidth, &wallImageHeight, &wallImageChannels, 0);
        BImage wallImage(WALL_TEXTURE.c_str());
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glActiveTexture(GL_TEXTURE0);
        
        // Cubemap texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, _tex);
        
        // Give the image to OpenGL (one for each face)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        // Format cube map texture
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    // Set custom uniform variables
    {
        _offsetUniform = glGetUniformLocation(_shaderProgram, "offset");
    }
}

void Corridor::customBindings()
{
    // Custom uniform variables
    glUniform1f(_offsetUniform, _offset);
    
    // Cubemap texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _tex);
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

/**
 * Get offset in corridor
 */
float Corridor::getOffset()
{
    return _offset;
}

/**
 * Set the offset. Make sure it always remains between 0 and 1
 */
void Corridor::increaseOffset(float increment)
{
    _offset += increment;
    // Return to (0,1)
    _offset = _offset - (int)_offset;
}