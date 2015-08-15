//
//  SkyBox.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "SkyBox.h"
#include "Camera.h"

static const std::string skyboxLF = "assets/skybox/skyboxLF.png";
static const std::string skyboxFT = "assets/skybox/skyboxFT.png";
static const std::string skyboxRT = "assets/skybox/skyboxRT.png";
static const std::string skyboxBK = "assets/skybox/skyboxBK.png";
static const std::string skyboxUP = "assets/skybox/skyboxUP.png";
static const std::string skyboxDN = "assets/skybox/skyboxDN.png";

SkyBox::SkyBox() :
RenderableSceneNode("SkyBoxShader", vec3(0.0f), quat(vec3(0.0f)), vec3(20.0f))
{
    // Create box
    const GLfloat vertices[] = {
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
    
    // Indices
    const GLuint indices[] = {
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

    // Push VBO
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setPositionsVBO(verticesVector);
    
    // Push IBO
    std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVector);
    
    // Add textures
    {
        _renderComponent->addTexture(TextureComponent::createCubeTexture(skyboxLF, skyboxFT, skyboxRT, skyboxBK, skyboxUP, skyboxDN));
    }
}

SkyBox::~SkyBox()
{
}

void SkyBox::update(float dt)
{
    setPosition(Camera::MainCamera()->getPosition());
}
