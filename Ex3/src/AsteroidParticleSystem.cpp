//
//  AsteroidParticleSystem.cpp
//  CGP-Ex3
//
//  The asteroid particle system.
//
//  Created by Amir Blum on 5/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "AsteroidParticleSystem.h"
#include "Camera.h"
#include <glm/gtc/random.hpp>

static const std::string ASTEROID_IMAGE = "assets/asteroid1.png";

AsteroidParticleSystem::AsteroidParticleSystem(int maxAsteroids, float asteroidRadius, float emitRadius, Ship *ship) :
ParticleSystem(maxAsteroids, "AsteroidShader", vec3(1.0f, 0.0f, 1.0f), quat(vec3(0.0f)), vec3(2.0f)),
_asteroidRadius(asteroidRadius),
_emitRadius(emitRadius),
_ship(ship),
_positions(maxAsteroids, _renderComponent, "particleCenter"),
_physics(maxAsteroids)
{
    // Render-related
    {
        // VBO
        static const GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f
        };
        
        std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
        _renderComponent->setVBO(verticesVector);
        
        // IBO
        static const GLubyte indices[] = {
            2, 1, 0,
            2, 3, 1
        };
        
        std::vector<GLubyte> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLubyte)));
        _renderComponent->setIBO(indicesVector);
        
        // Texture
        _renderComponent->add2DTexture(ASTEROID_IMAGE);
    }
    
    
    // Add particle attributes
    addAttribute(&_positions);
    addShaderAttribute(&_positions);
    
    addAttribute(&_physics);
    
    // Emit all the asteroids
    for (int i = 0; i < maxAsteroids; ++i) {
//        // Create random vector on unit sphere
//        vec3 newPosition = sphericalRand(1.0f);
//        
//        // Make it's length be between [0,_emitRadius]
//        float length = ((float)rand() / RAND_MAX) * (_emitRadius);
//        newPosition *= length;
//        
//        _positionAttribute[i] = newPosition;
        emit();
    }
}

AsteroidParticleSystem::~AsteroidParticleSystem()
{
}

void AsteroidParticleSystem::emit()
{
//    // Create random direction
//    float azimuth = ((float)rand() / RAND_MAX) * 2 * pi<float>();
//    vec2 xy = vec2(cos(azimuth), sin(azimuth));
//    float z = (2 * ((float)rand() / RAND_MAX)) - 1; // z is in the range [-1,1]
//    vec2 planar = xy * sqrt(1 - z * z);
//    vec3 newPosition(planar, z);
//    // (I think newPosition is already normalized, but the math is a tad shakey.
//    // This is just in case)
//    newPosition = normalize(newPosition);
//    
    // Create a particle
    int particleID = createNewParticle();
    if (particleID == -1) {
        return;
    }
    
    // Position attribute
    {
        // Create random vector on unit sphere
        vec3 newPosition = sphericalRand(1.0f);
        
        // Make it's length be between [0,_emitRadius]
        float length = ((float)rand() / RAND_MAX) * (_emitRadius);
        newPosition *= length;
        
        // Make sure it's behind the ship
        if (dot(newPosition, _ship->getForward()) > 0) {
            newPosition = -newPosition;
        }
        
        _positions[particleID] = newPosition;
    }
    
    // Physics attribute
    {
        _physics[particleID] = new PhysicsComponent(0.5f, 0.1f, 0.0f);
        vec3 direction = sphericalRand(1.0f);
        _physics[particleID]->applyForce(direction * 8.0f);
    }
}

void AsteroidParticleSystem::updateParticle(int particleID, float dt)
{
//    return;
    _physics[particleID]->update(dt);
    _positions[particleID] += _physics[particleID]->getVelocity();
    
    // Check death
    vec3 newPosition = _positions[particleID];
    if (length(_ship->getPosition() - newPosition) > _emitRadius &&
        dot(newPosition, _ship->getForward()) < 0) {
        killParticle(particleID);
        emit();
    }
}

void AsteroidParticleSystem::updateUniforms()
{
    Camera *camera = Camera::MainCamera();
    
    vec3 cameraUp = camera->getUp();
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraUp", cameraUp);
    
    vec3 cameraRight = cross(camera->getDirection(), cameraUp);
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraRight", cameraRight);
}