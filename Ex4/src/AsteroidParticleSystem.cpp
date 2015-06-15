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

AsteroidParticleSystem::AsteroidParticleSystem(vec3 center, quat axisRotation, unsigned int maxAsteroids, float emitRadius) :
ParticleSystem(maxAsteroids),
_center(center), _axisRotation(toMat4(axisRotation)),
_emitMaxRadius(emitRadius), _emitMinRadius(emitRadius * 4.0f / 5.0f),
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
    
//    // Emit min-radius
//    _emitMinRadius = glm::length(Camera::MainCamera()->getPosition()) * 2.0f;
    
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

vec3 AsteroidParticleSystem::asteroidToWorldPosition(vec3 asteroidPosition)
{
    vec4 affinePosition = vec4(asteroidPosition, 1.0f);
    vec4 affineWorldPosition = getWorldTransform() * affinePosition;
    return vec3(affineWorldPosition);
}

vec3 AsteroidParticleSystem::worldToAsteroidPosition(vec3 worldPosition)
{
    vec4 affinePosition = vec4(worldPosition, 1.0f);
    vec4 affineAsteroidPosition = inverse(getWorldTransform()) * affinePosition;
    return vec3(affineAsteroidPosition);
}

vec3 AsteroidParticleSystem::rotateAxis(vec3 vector)
{
    vec4 affineVector = vec4(vector, 1.0f);
    vec4 affineRotatedVector = _axisRotation * affineVector;
    return vec3(affineRotatedVector);
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
        randomPosition = vec3(circularRand(1.0f), 0.0f);
        
        // Decide it's length
        distance = randutils::randomRange(_emitMinRadius, _emitMaxRadius);
        
        // Make it relative to the center and set it's distance
        newPosition = _center + randomPosition * distance;
    }
    
    // Velocity
    vec3 velocity;
    {
        velocity = cross(-randomPosition, vec3(0.0f, 0.0f, 1.0f)) * 50.0f;
//        velocity = vec3(0.0f);
    }
    
    // Size
    float size;
    {
        size = randutils::randomRange(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE);
    }
    
    // Physics
    AsteroidRigidBody *newRigidBody;
    {
        // Initial velocity
//        vec3 randomDirection = sphericalRand(1.0f);
//        float randomSpeed = randutils::randomRange(ASTEROID_MIN_INITIAL_FORCE, ASTEROID_MAX_INITIAL_FORCE);
//        vec3 initialVelocity = randomDirection * randomSpeed;
        
//        bool closeToShip = distance < _emitMinRadius * 3.0f;
//        bool headingTowardsShip = dot(randomPosition, randomDirection) < -0.8;
//        if (closeToShip && headingTowardsShip) {
//            initialVelocity = -initialVelocity;
//        }
        
//        newPosition += initialVelocity;
        
//        newRigidBody = new AsteroidRigidBody(newPosition, initialVelocity, size * 0.3f, ASTEROID_MASS);
        newRigidBody = new AsteroidRigidBody(newPosition, velocity, size * 0.3f, size * ASTEROID_MASS / ASTEROID_MAX_SIZE);
    }
    
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
    AsteroidRigidBody *rigidBody = _rigidBodies.getValue(particleID);
    PhysicsComponent &physics = rigidBody->getPhysics();
    vec3 newWorldPosition = physics.getPosition();
    
    if (rigidBody->isDead()) {
        // Create random direction
        vec3 randomDirection = sphericalRand(1.0f);
        
        // Move it behind ship
        if (dot(randomDirection, Camera::MainCamera()->getDirection()) > 0) {
            randomDirection = -randomDirection;
        }
        
        newWorldPosition = randomDirection * _emitMaxRadius * 10.0f;
        
        // Create new rigidbody
        AsteroidRigidBody *newRigidBody = new AsteroidRigidBody(newWorldPosition, physics.getVelocity(), rigidBody->getRadius(), physics.getMass());
        delete rigidBody;
        _rigidBodies.setValue(particleID, newRigidBody);
    }

    _positions.setValue(particleID, newWorldPosition);
}