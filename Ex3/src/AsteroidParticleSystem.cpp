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

#define ASTEROID_MAX_VELOCITY (0.05f)

static const std::string ASTEROID_IMAGE = "assets/asteroid1.png";

AsteroidParticleSystem::AsteroidParticleSystem(int maxAsteroids, float asteroidRadius, float emitRadius, Ship *ship) :
ParticleSystem(maxAsteroids, "AsteroidShader"),
_asteroidRadius(asteroidRadius),
_emitMaxRadius(emitRadius), _emitMinRadius(0),
_ship(ship),
_positions(maxAsteroids, _renderComponent, "particleCenter"),
_physics(maxAsteroids),
_collided(maxAsteroids)
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
    addAttribute(&_collided);
    
    // Emit min-radius
    _emitMinRadius = glm::length(Camera::MainCamera()->getPosition() - _ship->getPosition()) * 2.0f;
    
    // Emit all the asteroids
    for (int i = 0; i < maxAsteroids; ++i) {
        emit();
    }
}

AsteroidParticleSystem::~AsteroidParticleSystem()
{
}

bool AsteroidParticleSystem::particleInView(vec3 particlePosition)
{
    Camera *camera = Camera::MainCamera();
    float cosAngle = dot(normalize(particlePosition), camera->getDirection());
    float particleAngle = acos(cosAngle);
    
    return particleAngle < radians(camera->getFrustumAngle());
}

void AsteroidParticleSystem::emit()
{
    // Create a particle
    int particleID = createNewParticle();
    if (particleID == -1) {
        return;
    }
    
    // Position
    vec3 randomPosition;
    float distance;
    vec3 newPosition;
    {
        // Create random vector on unit sphere
        randomPosition = sphericalRand(1.0f);
        
        // Decide it's length
        distance = ((float)rand() / RAND_MAX) * (_emitMaxRadius - _emitMinRadius) + _emitMinRadius;
        
        // Make it relative to the camera and set it
        newPosition = Camera::MainCamera()->getPosition() + randomPosition * distance;
    }
    
    // Physics
    PhysicsComponent *newPhysics;
    {
        vec3 randomDirection = sphericalRand(1.0f);
        vec3 initialForce = randomDirection * 8.0f;
        
        bool closeToShip = distance < _emitMinRadius * 3.0f;
        bool headingTowardsShip = dot(randomPosition, randomDirection) < -0.8;
        if (closeToShip && headingTowardsShip) {
            initialForce = -initialForce;
        }
        
        newPhysics = new PhysicsComponent(ASTEROID_MAX_VELOCITY);
        newPhysics->applyForce(initialForce);
    }
    
    _positions.setValue(particleID, newPosition);
    _physics.setValue(particleID, newPhysics);
}

void AsteroidParticleSystem::updateParticle(int particleID, float dt)
{
    // Update position
    vec3 currentPosition = _positions.getValue(particleID);
    vec3 updatedPosition = currentPosition;
    PhysicsComponent *physics = _physics.getValue(particleID);
    
    physics->update(dt);
    updatedPosition += physics->getVelocity();
    
    // Check death
    vec3 shipWorldPosition = _ship->getWorldPosition();
    vec3 relativePosition = updatedPosition - shipWorldPosition;
    if (length(relativePosition) > _emitMaxRadius) {
        // Pop-through to other side of field
        vec3 newRelativePosition = -relativePosition;
        updatedPosition = shipWorldPosition + newRelativePosition;
    }
    
    // Check collision
    if (length(relativePosition) < (_asteroidRadius + _ship->getRadius())) {
        bool collided = _collided.getValue(particleID);
        if (!collided) {
            _ship->collide();
            _collided.setValue(particleID, true);
        }
    } else {
        _collided.setValue(particleID, false);
    }
        
    // Update particle
    _positions.setValue(particleID, updatedPosition);
}

void AsteroidParticleSystem::updateUniforms()
{
    Camera *camera = Camera::MainCamera();
    
    vec3 cameraUp = camera->getUp();
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraUp", cameraUp);
    
    vec3 cameraRight = cross(camera->getDirection(), cameraUp);
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraRight", cameraRight);
}