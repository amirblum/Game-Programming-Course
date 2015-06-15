//
//  Shield.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Shield.h"

#define FINENESS (30)

static const std::string HEXMAP = "assets/hexmap.png";

Shield::Shield(vec3 pos, float radius) :
SceneNode(pos, quat(vec3(0.0f)), vec3(radius)),
_renderComponent("ShieldShader"),
_time(0.0f)
{
    int stacks = (int)radius * FINENESS;
    int slices = (int)radius * FINENESS;
    
    // VBO
    {
        std::vector<GLfloat> vertices;
        
        // Calc The Vertices
        for (unsigned int i = 0; i <= stacks; ++i) {
            float v = i / (float)stacks;
            float phi = v * pi<float>();
            
            // Loop Through Slices
            for (int j = 0; j <= slices; ++j){
                float u = j / (float)slices;
                float theta = u * (pi<float>() * 2);
                
                // Calc The Vertex Positions
                float x = cosf(theta) * sinf(phi);
                float y = cosf(phi);
                float z = sinf(theta) * sinf(phi);
                
                // Push Back Vertex Data
    //            vertices.push_back(vec3(x, y, z) * radius);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                vertices.push_back(1.0f);
            }
        }
        
        _renderComponent.setPositionsVBO(vertices);
    }
    
    // IBO
    {
        std::vector<GLuint> indices;
        
        // Calc The Index Positions
        for (unsigned int i = 0; i < slices * stacks + slices; ++i) {
            
            indices.push_back(i);
            indices.push_back(i + slices + 1);
            indices.push_back(i + slices);
            
            indices.push_back(i + slices + 1);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        
        _renderComponent.setIBO(indices);
    }
    
    // Texture
    {
        _renderComponent.addTexture(TextureComponent::createCubeTexture(HEXMAP, HEXMAP, HEXMAP, HEXMAP, HEXMAP, HEXMAP));
    }
    
//    // Mode
//    {
//        _renderComponent->setPolygonDrawMode(GL_LINE);
//    }
}

Shield::~Shield() {}

void Shield::update(float dt)
{
    _time += dt;
}

void Shield::render()
{
    glEnable(GL_CULL_FACE);
    _renderComponent.setUniform<float, UNIFORM_FLOAT>("time", _time);
    
    glCullFace(GL_FRONT);
    _renderComponent.render(_worldTransform);
    
    glCullFace(GL_BACK);
    _renderComponent.render(_worldTransform);
    
    glDisable(GL_CULL_FACE);
}