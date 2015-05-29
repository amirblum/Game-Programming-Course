//
//  AsteroidParticleSystem.cpp
//  CGP-Ex4
//
//  The asteroid particle system.
//
//  Created by Amir Blum on 5/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "AsteroidParticleSystem.h"
#include "Camera.h"
#include "RandUtils.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/random.hpp>

#define ASTEROID_MAX_VELOCITY (15.0f)
#define ASTEROID_MIN_VELOCITY (2.5f)
#define ASTEROID_MIN_SIZE (2.0f)
#define ASTEROID_MAX_SIZE (35.0f)
#define TRANSPARENCY_MARGIN (100.0f)

static const std::string ASTEROID_IMAGE = "assets/asteroid1.png";

AsteroidParticleSystem::AsteroidParticleSystem(unsigned int maxAsteroids, float emitRadius, Ship *ship) :
ParticleSystem(maxAsteroids),
_emitMaxRadius(emitRadius), _emitMinRadius(0),
_ship(ship),
_physics(maxAsteroids, "physics"),
_collided(maxAsteroids, "collided")
{
    // Render-related
    {
        // Texture
        _renderComponent->addTexture(TextureComponent::create2DTexture(ASTEROID_IMAGE));
    }
    
    // Add particle attributes
    {        
        addAttribute(&_physics);
        addAttribute(&_collided);
    }
    
    // Emit min-radius
    _emitMinRadius = glm::length(Camera::MainCamera()->getPosition() - _ship->getPosition()) * 2.0f;
    
    // Emit all the asteroids
    for (int i = 0; i < maxAsteroids; ++i) {
        emit();
    }
}

AsteroidParticleSystem::~AsteroidParticleSystem()
{
    for (unsigned int i = 0; i < _maxParticles; ++i) {
        delete _physics.getValue(i);
    }
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
        distance = randutils::randomRange(_emitMinRadius, _emitMaxRadius);
        
        // Make it relative to the camera and set it
        newPosition = Camera::MainCamera()->getPosition() + randomPosition * distance;
    }
    
    // Size
    float size;
    {
        size = randutils::randomRange(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE);
    }
    
    // Physics
    PhysicsComponent *newPhysics;
    {
        vec3 randomDirection = sphericalRand(1.0f);
        float randomSpeed = randutils::randomRange(ASTEROID_MIN_VELOCITY, ASTEROID_MAX_VELOCITY);
        vec3 initialForce = randomDirection * randomSpeed;
        
        bool closeToShip = distance < _emitMinRadius * 3.0f;
        bool headingTowardsShip = dot(randomPosition, randomDirection) < -0.8;
        if (closeToShip && headingTowardsShip) {
            initialForce = -initialForce;
        }
        
        newPhysics = new PhysicsComponent(ASTEROID_MAX_VELOCITY);
        newPhysics->applyForce(initialForce);
    }
    
    // Random fun
    // To avoid having most asteroids near the ship at start (due to the way
    // the position was randomly chosen), we let the physics run a bit
    newPosition += newPhysics->getVelocity() * 30.0f;
    
    // Billboard right
    vec2 newBillboardRight = circularRand(1.0f);    
    
    _positions.setValue(particleID, newPosition);
    _sizes.setValue(particleID, size);
    _physics.setValue(particleID, newPhysics);
    _collided.setValue(particleID, false);
    _transparencies.setValue(particleID, 1.0f);
    _billboardRights.setValue(particleID, newBillboardRight);
}

void AsteroidParticleSystem::updateParticle(unsigned int particleID, float dt)
{
    // Update position
    vec3 currentPosition = _positions.getValue(particleID);
    vec3 updatedPosition = currentPosition;
    PhysicsComponent *physics = _physics.getValue(particleID);
    
    updatedPosition += physics->getVelocity() * dt;
    
    // Check death
    vec3 shipWorldPosition = _ship->getWorldPosition();
    vec3 relativePosition = updatedPosition - shipWorldPosition;
    float distanceFromShip = length(relativePosition);
    if (distanceFromShip > _emitMaxRadius) {
        // Pop-through to other side of field
        vec3 newRelativePosition = -relativePosition;
        updatedPosition = shipWorldPosition + newRelativePosition;
    }
    
    // Check collision
    if (distanceFromShip < (_sizes.getValue(particleID) * 0.3f + _ship->getRadius())) {
        bool collided = _collided.getValue(particleID);
        if (!collided) {
            _ship->collide();
            _collided.setValue(particleID, true);
        }
    } else {
        _collided.setValue(particleID, false);
    }
    
    // Transparency
    float transparency = 1.0f;
    if (distanceFromShip > (_emitMaxRadius - TRANSPARENCY_MARGIN)) {
        transparency = 1.0f - (distanceFromShip - (_emitMaxRadius - TRANSPARENCY_MARGIN)) / TRANSPARENCY_MARGIN;
    }
    _transparencies.setValue(particleID, transparency);
    
    // Update particle
    _positions.setValue(particleID, updatedPosition);
}