//
//  SkyBox.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "SkyBox.h"

static const std::string skyboxLF = "assets/skyboxLF.png";
static const std::string skyboxFT = "assets/skyboxFT.png";
static const std::string skyboxRT = "assets/skyboxRT.png";
static const std::string skyboxBK = "assets/skyboxBK.png";
static const std::string skyboxUP = "assets/skyboxUP.png";
static const std::string skyboxDN = "assets/skyboxDN.png";

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
    const GLubyte indices[] = {
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
    _renderComponent->setVBO(verticesVector);
    
    // Push IBO
    std::vector<GLubyte> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLubyte)));
    _renderComponent->setIBO(indicesVector);
    
    // Add textures
    {
        _renderComponent->addCubeTexture(skyboxLF, skyboxFT, skyboxRT, skyboxBK, skyboxUP, skyboxDN);
    }
}

SkyBox::~SkyBox()
{
}
