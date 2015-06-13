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

#define ASTEROID_MASS (2.0f)
#define ASTEROID_MAX_INITIAL_FORCE (15.0f)
#define ASTEROID_MIN_INITIAL_FORCE (2.0f)
#define ASTEROID_MIN_SIZE (20.0f)
#define ASTEROID_MAX_SIZE (150.0f)
#define TRANSPARENCY_MARGIN (100.0f)

static const std::string ASTEROID_IMAGE = "assets/asteroid1.png";

AsteroidParticleSystem::AsteroidParticleSystem(unsigned int maxAsteroids, float emitRadius) :
ParticleSystem(maxAsteroids),
_emitMaxRadius(emitRadius), _emitMinRadius(0),
//_ship(ship),
_rigidBodies(maxAsteroids, "rigidBodies")
{
    // Render-related
    {
        // Texture
        _renderComponent->addTexture(TextureComponent::create2DTexture(ASTEROID_IMAGE));
    }
    
    // Add particle attributes
    {        
        addAttribute(&_rigidBodies);
    }
    
    // Emit min-radius
    _emitMinRadius = glm::length(Camera::MainCamera()->getPosition()) * 2.0f;
    
    // Emit all the asteroids
    for (int i = 0; i < maxAsteroids; ++i) {
        emit();
    }
}

AsteroidParticleSystem::~AsteroidParticleSystem()
{
    for (unsigned int i = 0; i < _maxParticles; ++i) {
        delete _rigidBodies.getValue(i);
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
    AsteroidRigidBody *newRigidBody;
    {
        vec3 randomDirection = sphericalRand(1.0f);
        float randomSpeed = randutils::randomRange(ASTEROID_MIN_INITIAL_FORCE, ASTEROID_MAX_INITIAL_FORCE);
        vec3 initialVelocity = randomDirection * randomSpeed;
        
        bool closeToShip = distance < _emitMinRadius * 3.0f;
        bool headingTowardsShip = dot(randomPosition, randomDirection) < -0.8;
        if (closeToShip && headingTowardsShip) {
            initialVelocity = -initialVelocity;
        }
        
        // Move the asteroid around a bit from the initial random position
//        while (distance < size) {
//            newPosition += initialVelocity * 30.0f;
//            distance = length(newPosition);
//        }
        newPosition += initialVelocity;
        
        newRigidBody = new AsteroidRigidBody(newPosition, initialVelocity, size * 0.3f, ASTEROID_MASS);
//        newRigidBody->getPhysics().applyForce(initialForce);
    }
    
    // Random fun
    // To avoid having most asteroids near the ship at start (due to the way
    // the position was randomly chosen), we let the physics run a bit
//    newPosition += newRigidBody->getPhysics().getVelocity() * 30.0f;
    
    // Billboard right
    vec2 newBillboardRight = circularRand(1.0f);    
    
    _positions.setValue(particleID, newPosition);
    _sizes.setValue(particleID, size);
    _rigidBodies.setValue(particleID, newRigidBody);
    _transparencies.setValue(particleID, 1.0f);
    _billboardRights.setValue(particleID, newBillboardRight);
}

void AsteroidParticleSystem::updateParticle(unsigned int particleID, float dt)
{
    // Update position
//    vec3 currentPosition = _positions.getValue(particleID);
//    vec3 newPosition = currentPosition;
    vec3 newPosition = _rigidBodies.getValue(particleID)->getPhysics().getPosition();
//    PhysicsComponent &physics = _rigidBodies.getValue(particleID)->getPhysics();
//    newPosition += physics.getVelocity() * dt;
    
    
    // Check death
//    vec3 shipWorldPosition = _ship->getWorldPosition();
//    vec3 relativePosition = newPosition - shipWorldPosition;
//    float distanceFromShip = length(relativePosition);
    float distanceFromCenter = length(newPosition);
//    if (distanceFromShip > _emitMaxRadius) {
//        // Pop-through to other side of field
//        vec3 newRelativePosition = -relativePosition;
//        newPosition = shipWorldPosition + newRelativePosition;
//    }
    
    // Check collision
//    if (distanceFromShip < (_sizes.getValue(particleID) * 0.3f + _ship->getRadius())) {
//        bool collided = _collided.getValue(particleID);
//        if (!collided) {
//            _ship->collide();
//            _collided.setValue(particleID, true);
//        }
//    } else {
//        _collided.setValue(particleID, false);
//    }
    
    // Transparency
    float transparency = 1.0f;
    if (distanceFromCenter > (_emitMaxRadius - TRANSPARENCY_MARGIN)) {
        transparency = 1.0f - (distanceFromCenter - (_emitMaxRadius - TRANSPARENCY_MARGIN)) / TRANSPARENCY_MARGIN;
    }
    _transparencies.setValue(particleID, transparency);
    
    // Update particle
    _positions.setValue(particleID, newPosition);
}