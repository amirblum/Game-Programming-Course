//
//  Ship.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Ship.h"
//#include "Camera.h"

#include <iostream>
//#include "stb_image.h"
//#include "bimage.h"

static const std::string SHIP_TEXTURE = "assets/wallTexture-squashed.bmp";
static const std::string SHIP_BUMP = "assets/wallTexture-squashed-bump.bmp";

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, vec3 rotation, vec3 scale) :
SceneNode(position, rotation, scale),
_renderComponent("ship", "ShipShader.vert", "ShipShader.frag")
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
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent.setVBO(verticesVector);
    
    
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
    
    std::vector<GLubyte> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLubyte)));
    _renderComponent.setIBO(indicesVector);
    
    // Create ship textures
    {
        _renderComponent.addTexture(SHIP_TEXTURE, GL_TEXTURE_2D);
        _renderComponent.addTexture(SHIP_BUMP, GL_TEXTURE_2D);
    }
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
}

void Ship::update(float dt)
{
    
}

void Ship::render()
{
    _renderComponent.render(_worldTransform);
}