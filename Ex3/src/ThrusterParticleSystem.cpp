//
//  ThrusterParticleSystem.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/11/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#define GLM_FORCE_RADIANS
#include <glm/gtc/random.hpp>
#include "ThrusterParticleSystem.h"
#include "RandUtils.h"

#define MIN_EMIT_TIME (0.001f)
#define MAX_EMIT_TIME (0.03f)
#define PARTICLE_SIZE (0.01f)
#define PARTICLE_VELOCITY (0.5f)
#define COLOR_THRESHOLD (0.8f)

static const std::string THRUSTER_IMAGE = "assets/particle.png";

ThrusterParticleSystem::ThrusterParticleSystem(unsigned int maxParticles,
                                               float thrusterRadius,
                                               vec3 position,
                                               quat rotation,
                                               vec3 scale) :
ParticleSystem(maxParticles, position, rotation, scale),
_thrusterRadius(thrusterRadius),
_originalDistances(maxParticles, "originalDistance"),
_nextEmitTime(0.0f),
_timeSinceLastEmission(0.0f)
{
    // Render-related
    {
        // Texture
        _renderComponent->addTexture(TextureComponent::create2DTexture(THRUSTER_IMAGE));
    }
}

ThrusterParticleSystem::~ThrusterParticleSystem()
{
    
}

void ThrusterParticleSystem::updateGeneral(float dt)
{
//    _timeSinceLastEmission += dt;
//    
//    if (_timeSinceLastEmission >= _nextEmitTime) {
//        _timeSinceLastEmission = 0.0f;
//        _nextEmitTime = randutils::randomRange(MIN_EMIT_TIME, MAX_EMIT_TIME);
//        
//        for (unsigned int i = 0; i < 100; ++i) {
//            emit();
//        }
//    }
}

void ThrusterParticleSystem::emit()
{
    int particleID = createNewParticle();
    if (particleID == -1) {
        return;
    }
    
    float distanceFromCenter = randutils::randomRange(0, _thrusterRadius);
    vec3 newPosition = vec3(circularRand(distanceFromCenter), 0.0f);
    
    float newSize = PARTICLE_SIZE;
    
    _positions.setValue(particleID, newPosition);
    _sizes.setValue(particleID, newSize);
    _transparencies.setValue(particleID, 1.0f);
    _tints.setValue(particleID, vec3(1.0f));
    _originalDistances.setValue(particleID, vec2(newPosition.x, newPosition.y));
}

void ThrusterParticleSystem::updateParticle(unsigned int particleID, float dt)
{
    vec3 position = _positions.getValue(particleID);
    
    position.z -= PARTICLE_VELOCITY * dt;
    
    if (-position.z > (_thrusterRadius * 0.5f)) {
        float percentToEnd = (-(position.z + _thrusterRadius * 0.5f) / _thrusterRadius);
        
        // Position
        float circularAmount = cos(percentToEnd);
        vec2 originalDistance = _originalDistances.getValue(particleID);
        position.x = originalDistance.x * circularAmount;
        position.y = originalDistance.y * circularAmount;
        
        // Tint
        vec3 tint = mix(vec3(1.0f), vec3(0.0f, 0.0f, 1.0f), percentToEnd);
        _tints.setValue(particleID, tint);
    }
    
    _positions.setValue(particleID, position);
    
    if (position.z < (-_thrusterRadius * 1.5f)) {
        killParticle(particleID);
    }
}