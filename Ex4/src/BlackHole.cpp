//
//  BlackHole.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "BlackHole.h"
#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/random.hpp>

#define BH_SIZE_MASS_RATIO (1.0f)
#define SPIN_SPEED (0.1f)

static const std::string BLACK_HOLE_IMAGE = "assets/blackhole.png";

BlackHole::BlackHole(vec3 position, float size) :
RenderableSceneNode("BlackHoleShader", position),
RigidBody(position, vec3(0.0f), size * 0.3f, size * size * BH_SIZE_MASS_RATIO, true),
_size(size), _spinAmount(0.0f), _right(circularRand(1.0f))
{
    // Set physics to static
    _physics.setStatic(true);
    
    // VBO
    static const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f
    };
    
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setPositionsVBO(verticesVector);
    
    // IBO
    static const GLuint indices[] = {
        2, 1, 0,
        2, 3, 1
    };
    
    std::vector<GLuint> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLuint)));
    _renderComponent->setIBO(indicesVector);
    
    // Texture
    _renderComponent->addTexture(TextureComponent::create2DTexture(BLACK_HOLE_IMAGE));
}

BlackHole::~BlackHole()
{
    
}

void BlackHole::update(float dt)
{
    _spinAmount += pi<float>() * SPIN_SPEED * dt;
    if (_spinAmount > pi<float>()) {
        _spinAmount = _spinAmount < pi<float>();
    }
    _right = vec2(sin(_spinAmount), cos(_spinAmount));
}

void BlackHole::preRender()
{
    _renderComponent->setUniform<float, UNIFORM_FLOAT>("size", _size);
    _renderComponent->setUniform<vec2, UNIFORM_VEC2>("billboardRight", _right);
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraWorldPosition", Camera::MainCamera()->getPosition());
}