//
//  Building.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Building.h"
#include "RandUtils.h"


static const std::string texturesLocation = "assets/buildings/";
static const std::string textures[] = {
    "building1.jpg",
    "building2.jpg"
};

Building::Building(vec3 position, float width, float height) :
RenderableSceneNode("BuildingShader", position),
_width(width), _height(height)
{
    // Initialize building vertices
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
    _renderComponent->setPositionsVBO(verticesVector);
    
    
    static const GLuint indices[] = {
        // Left
        0, 1, 2,
        1, 3, 2,
        
        // Front
        2, 3, 6,
        3, 7, 6,
        
        // Right
        5, 4, 7,
        4, 6, 7,
        
        // Back
        4, 5, 0,
        5, 1, 0,
        
        // Up
        1, 5, 3,
        3, 7, 5,
        
        // Down
        4, 0, 6,
        0, 2, 6
    };
    
    std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVector);
    
    // Initialize randomness
    {
        // Height
        setScale(vec3(width, height, width));
        
        vec3 myPosition = getPosition();
        myPosition.y = _height / 2;
        setPosition(myPosition);
        
        // Texture
        std::string texture = texturesLocation + "/" + textures[randutils::randomRange(0, 2)];
        _renderComponent->addTexture(TextureComponent::createCubeTexture(texture, texture, texture, texture, texture, texture));
    }
}

Building::~Building()
{
    
}