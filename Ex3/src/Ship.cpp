//
//  Ship.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Ship.h"
#include "Camera.h"

#include <iostream>
//#include "stb_image.h"
#include "bimage.h"

static const std::string WALL_TEXTURE = "assets/wallTexture-squashed.bmp";
static const std::string SHIP_BUMP = "assets/wallTexture-squashed-bump.bmp";

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, vec3 rotation, vec3 scale) :
Renderable("ship", "ShipShader.vert", "ShipShader.frag", position, rotation, scale)
{
    // Initialize ship
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
    
    // Create ship texture
    {
        // Load wall image
//        int wallImageWidth, wallImageHeight, wallImageChannels;
//        unsigned char *wallImageData = stbi_load(WALL_TEXTURE.c_str(), &wallImageWidth, &wallImageHeight, &wallImageChannels, 0);
        BImage shipMap;
        if (!shipMap.readImage(WALL_TEXTURE.c_str())) {
            std::cout << "Failed to read ship texture" << std::endl;
            exit(1);
        }
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glActiveTexture(GL_TEXTURE0);
        
        // Cubemap texture
        glBindTexture(GL_TEXTURE_2D, _tex);
        
        // Give the image to OpenGL (one for each face)
        glTexImage2D(GL_TEXTURE_2D, 0,
                     GL_RGBA,
                     shipMap.width(), shipMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     shipMap.getImageData());
        
        // Format cube map texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // Load the bump map
        BImage shipBumpMap;
        if (!shipBumpMap.readImage(SHIP_BUMP.c_str())) {
            std::cout << "Failed to read ship texture bump" << std::endl;
            exit(1);
        }
        
        glActiveTexture(GL_TEXTURE1);
        
        glBindTexture(GL_TEXTURE_2D, _bump); // Bind texture before setting its properties
        
        // Give the image to OpenGL (one for each face)
        glTexImage2D(GL_TEXTURE_2D, 0,
                     GL_RGBA,
                     shipBumpMap.width(), shipBumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     shipBumpMap.getImageData());
        
        
        // Format cube map texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    // Set custom uniform variables
    {
        _lightPosUniform = glGetUniformLocation(_shaderProgram, "lightPos");
        _lightDirUniform = glGetUniformLocation(_shaderProgram, "lightDir");
    }
}

void Ship::customBindings()
{
    // Custom uniform variables
    glUniform3fv(_lightPosUniform, 1, value_ptr(_lightPos));
    glUniform3fv(_lightDirUniform, 1, value_ptr(_lightDir));
    
    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _bump);
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
}

/**
 * Sets the light position
 */
void Ship::setLightPos(vec3 lightPos)
{
    _lightPos = lightPos;
}

/**
 * Sets the light direction
 */
void Ship::setLightDir(vec3 lightDir)
{
    _lightDir = lightDir;
}