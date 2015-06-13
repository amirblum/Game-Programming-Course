//
//  Overlay.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Overlay.h"

Overlay::Overlay(std::string texture) :
RenderableSceneNode("HUDShader")
{
    // Initialize board
    static const GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f,
        
        1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, -1.0f, 1.0f
    };
    
    
    // Push VBO
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setPositionsVBO(verticesVector);
    
    
    static const GLuint indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    
    std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVector);
    
    _renderComponent->addTexture(TextureComponent::create2DTexture(texture));
}

Overlay::~Overlay()
{
    
}