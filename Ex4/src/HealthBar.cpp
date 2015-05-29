//
//  HealthBar.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "HealthBar.h"

HealthBar::HealthBar(int maxUnits, vec3 position, quat rotation, vec3 scale) :
RenderableSceneNode("HealthBarShader", position, rotation, scale),
_maxUnits(maxUnits), _currentUnits(maxUnits)
{
    const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f,
        0.5, 0.5f, 0.0f, 1.0f
    };
    
    std::vector<GLfloat> verticesVec(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setPositionsVBO(verticesVec);
    
    const GLuint indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    
    std::vector<GLuint> indicesVec(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVec);
}

HealthBar::~HealthBar()
{
}

void HealthBar::preRender()
{
    _renderComponent->setUniform<int, UNIFORM_INT>("maxUnits", _maxUnits);
    _renderComponent->setUniform<int, UNIFORM_INT>("currentUnits", _currentUnits);
}

int HealthBar::getCurrentUnits()
{
    return _currentUnits;
}

void HealthBar::setCurrentUnits(int currentUnits)
{
    _currentUnits = currentUnits;
}