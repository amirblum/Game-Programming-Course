//
//  DummyObject.cpp
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ShaderIO.h"
#include "DummyObject.h"
#include "InputManager.h"

#include <iostream>

static const std::string DummyObject_TEXTURE = "assets/wallTexture-squashed.bmp";
static const std::string DummyObject_BUMP = "assets/wallTexture-squashed-bump.bmp";

#define TURN_SPEED (0.3f)

/**
 * DummyObject constructor
 */
DummyObject::DummyObject(vec3 position, quat rotation, vec3 scale) :
SceneNode(position, rotation, scale),
_renderComponent("ShipShader")
{
    // Initialize DummyObject
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
    
    // Create DummyObject textures
    {
        _renderComponent.add2DTexture(DummyObject_TEXTURE);
        _renderComponent.add2DTexture(DummyObject_BUMP);
    }
}

/**
 * Deconstructor
 */
DummyObject::~DummyObject()
{
}

void DummyObject::update(float dt)
{
}

void DummyObject::render()
{
    _renderComponent.render(_worldTransform);
}