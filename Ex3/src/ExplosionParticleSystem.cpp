//
//  ExplosionParticleSystem.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/11/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#define GLM_FORCE_RADIANS
#include <glm/gtc/random.hpp>
#include "ExplosionParticleSystem.h"
#include "RandUtils.h"

#define MIN_EMIT_TIME (0.001f)
#define MAX_EMIT_TIME (0.03f)
#define MIN_SIZE (0.1f)
#define MAX_SIZE (1.0f)
#define MIN_VELOCITY (0.02f)
#define MAX_VELOCITY (0.1f)
#define DAMPENING_SPEED (0.1f)
#define SHRINK_SPEED (1.0f)
#define FADE_SPEED (1.0f)

static const std::string EXPLOSION_IMAGE = "assets/fireball.png";

ExplosionParticleSystem::ExplosionParticleSystem(unsigned int maxParticles,
                                                 vec3 position,
                                                 quat rotation,
                                                 vec3 scale) :
ParticleSystem(maxParticles, position, rotation, scale),
_physics(maxParticles, "physics"),
_nextEmitTime(0.0f),
_timeSinceLastEmission(0.0f),
_particlesEmitted(0)
{
    // Render-related
    {
        // Texture
        _renderComponent->addTexture(TextureComponent::create2DTexture(EXPLOSION_IMAGE));
    }
    
    // Add particle attributes
    {
        addAttribute(&_physics);
    }
}

ExplosionParticleSystem::~ExplosionParticleSystem()
{
    
}

void ExplosionParticleSystem::updateGeneral(float dt)
{
    if (_particlesEmitted >= _maxParticles) {
        return;
    }
    
    _timeSinceLastEmission += dt;
    
    if (_timeSinceLastEmission >= _nextEmitTime) {
        _timeSinceLastEmission = 0.0f;
        _nextEmitTime = randutils::randomRange(MIN_EMIT_TIME, MAX_EMIT_TIME);

        emit();
        _particlesEmitted++;
    }
}

void ExplosionParticleSystem::emit()
{
    int particleID = createNewParticle();
    if (particleID == -1) {
        return;
    }
    
    float newSize = randutils::randomRange(MIN_SIZE, MAX_SIZE);
    
    PhysicsComponent *newPhysics = new PhysicsComponent(MAX_VELOCITY);
    float force = randutils::randomRange(MIN_VELOCITY, MAX_VELOCITY);
    vec3 directionalForce = sphericalRand(force);
    newPhysics->applyForce(directionalForce);
    
    _positions.setValue(particleID, vec3(0.0f));
    _sizes.setValue(particleID, newSize);
    _physics.setValue(particleID, newPhysics);
    _transparencies.setValue(particleID, 1.0f);
}

void ExplosionParticleSystem::updateParticle(unsigned int particleID, float dt)
{
    vec3 position = _positions.getValue(particleID);
    float size = _sizes.getValue(particleID);
    float transparency = _transparencies.getValue(particleID);
    PhysicsComponent *physics = _physics.getValue(particleID);
    vec3 velocity = physics->getVelocity();
    
    position += velocity;
    physics->applyForce(-velocity * DAMPENING_SPEED * dt);
    
    size -= SHRINK_SPEED * dt;
    transparency -= FADE_SPEED * dt;
    
    _positions.setValue(particleID, position);
    _sizes.setValue(particleID, size);
    _transparencies.setValue(particleID, transparency);
    
    if (size <= 0 || transparency <= 0) {
        killParticle(particleID);
    }
}

bool ExplosionParticleSystem::isDead()
{
    return _particlesEmitted == _maxParticles && _aliveParticles == 0;
}