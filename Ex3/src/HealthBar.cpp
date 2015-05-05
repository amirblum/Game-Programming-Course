//
//  HealthBar.cpp
//  CGP-Ex3
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
    _renderComponent->setVBO(verticesVec);
    
    const GLubyte indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    
    std::vector<GLubyte> indicesVec(indices, indices + (sizeof(indices) / sizeof(GLubyte)));
    _renderComponent->setIBO(indicesVec);
}

HealthBar::~HealthBar()
{
}

void HealthBar::updateUniforms()
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