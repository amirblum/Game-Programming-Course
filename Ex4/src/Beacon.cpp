//
//  Beacon.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/4/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Beacon.h"
#include "Camera.h"

static const std::string BEACON_MESH = "assets/battlestar/battlestar.dae";
static const std::string COMPASS_TARGET = "assets/target.png";

Beacon::Beacon(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
RigidBody(position, vec3(0.0f), radius, radius, false),
_compass("CompassShader")
{
    // Set up physics
    {
        _physics.setStatic(true);
    }
    
    // Set up mesh
    {
        vec3 meshPosition = vec3(0.0f);
        quat meshRotation = rotate(quat(vec3(0.0f)), pi<float>()/2.0f, vec3(0.0f, 0.0f, 1.0f));
        meshRotation = rotate(meshRotation, pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
    //    quat meshRotation = quat(vec3(0.0f));
        vec3 meshScale = vec3(1.0f);
        
        _mesh = new Mesh(BEACON_MESH, meshPosition, meshRotation, meshScale);
        addChild(_mesh);
    }
    
    // Compass
    {
        // Initialize board
        static const GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,
            
            0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f
        };
        
        
        // Push VBO
        std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
        _compass.setPositionsVBO(verticesVector);
        
        
        static const GLuint indices[] = {
            0, 1, 2,
            1, 3, 2
        };
        
        std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
        _compass.setIBO(indicesVector);

        
        _compass.addTexture(TextureComponent::create2DTexture(COMPASS_TARGET));
    }
}

Beacon::~Beacon() {}

void Beacon::render()
{
    Camera *camera = Camera::MainCamera();
    _compass.setUniform<vec3, UNIFORM_VEC3>("beaconPosition", getWorldPosition());
    _compass.setUniform<mat4, UNIFORM_MAT4>("view", camera->getView());
    _compass.setUniform<mat4, UNIFORM_MAT4>("projection", camera->getProjection());
    _compass.setUniform<float, UNIFORM_FLOAT>("aspectRatio", camera->getAspectRatio());
    _compass.render(_worldTransform);
}