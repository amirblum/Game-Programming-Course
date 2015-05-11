//
//  GameOver.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "GameOver.h"

static const std::string GAME_OVER_TEXTURE = "assets/gameover.png";

GameOver::GameOver() :
RenderableSceneNode("HUDShader")
{
    // Initialize board
    static const GLfloat vertices[] = {
        -1.0f, -0.5f, -1.0f, 1.0f,
        -1.0f, 0.5f, -1.0f, 1.0f,
        
        1.0f, -0.5f, -1.0f, 1.0f,
        1.0f, 0.5f, -1.0f, 1.0f
    };
    
    
    // Push VBO
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setVBO(verticesVector);
    
    
    static const GLuint indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    
    std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVector);
    
    _renderComponent->addTexture(TextureComponent::create2DTexture(GAME_OVER_TEXTURE));
}

GameOver::~GameOver()
{
    
}