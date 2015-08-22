//
//  BoundingBoxRenderer.cpp
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ShaderIO.h"
#include "BoundingBoxRenderer.h"
#include "InputManager.h"

#include <iostream>

static const vec4 BB_COLOR = vec4(0.0f, 1.0f, 0.0f, 1.0f);

/**
 * BoundingBoxRenderer constructor
 */
BoundingBoxRenderer::BoundingBoxRenderer(BoundingBox &bb) :
RenderableSceneNode("SimpleShader")
{
    initWithBoundingBox(bb);
    
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
    
    // Push IBO
    std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVector);
    
    _renderComponent->setPolygonDrawMode(GL_LINE);
}

void BoundingBoxRenderer::initWithBoundingBox(BoundingBox &bb)
{
    vec3 halfsize = bb.getSize() * 0.5f;
    
    // Create box
    //    const GLfloat vertices[] = {
    //        // Left wall
    //        bb.minX(), bb.minY(), bb.maxZ(), 1.0f,
    //        bb.minX(), bb.maxY(), bb.maxZ(), 1.0f,
    //
    //        bb.minX(), bb.minY(), bb.minZ(), 1.0f,
    //        bb.minX(), bb.maxY(), bb.minZ(), 1.0f,
    //
    //        // Right wall
    //        bb.maxX(), bb.minY(), bb.maxZ(), 1.0f,
    //        bb.maxX(), bb.maxY(), bb.maxZ(), 1.0f,
    //
    //        bb.maxX(), bb.minY(), bb.minZ(), 1.0f,
    //        bb.maxX(), bb.maxY(), bb.minZ(), 1.0f
    //    };
    
    
    // Create box
    const GLfloat vertices[] = {
        // Left wall
        -halfsize.x, -halfsize.y, halfsize.z, 1.0f,
        -halfsize.x, halfsize.y, halfsize.z, 1.0f,
        
        -halfsize.x, -halfsize.y, -halfsize.z, 1.0f,
        -halfsize.x, halfsize.y, -halfsize.z, 1.0f,
        
        // Right wall
        halfsize.x, -halfsize.y, halfsize.z, 1.0f,
        halfsize.x, halfsize.y, halfsize.z, 1.0f,
        
        halfsize.x, -halfsize.y, -halfsize.z, 1.0f,
        halfsize.x, halfsize.y, -halfsize.z, 1.0f
    };
    
    // Push VBO
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setPositionsVBO(verticesVector);
}

/**
 * Deconstructor
 */
BoundingBoxRenderer::~BoundingBoxRenderer()
{
}

void BoundingBoxRenderer::setBoundingBox(BoundingBox &bb)
{
    initWithBoundingBox(bb);
}

void BoundingBoxRenderer::preRender()
{
    _renderComponent->setUniform<vec4, UNIFORM_VEC4>("fillColor", BB_COLOR);
}