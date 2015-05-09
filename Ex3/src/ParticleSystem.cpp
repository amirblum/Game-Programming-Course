//
//  ParticleSystem.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/30/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxParticles, std::string shaderProgram,
                               vec3 position, quat rotation, vec3 scale) :
RenderableSceneNode(shaderProgram, position, rotation, scale),
_maxParticles(maxParticles),
_allParticleAttributes(),
_shaderAttributes(),
_aliveParticles(0) {}

ParticleSystem::~ParticleSystem()
{
    for (ParticleAttribute *attribute : _allParticleAttributes) {
        delete attribute;
    }
}

void ParticleSystem::addAttribute(ParticleAttribute *attribute)
{
    _allParticleAttributes.push_back(attribute);
}

void ParticleSystem::addShaderAttribute(ShaderAttribute *attribute)
{
    _shaderAttributes.push_back(attribute);
}

/**
 * Give the next ID.
 * Returns -1 if no more room in the system.
 */
int ParticleSystem::createNewParticle()
{
    if (_aliveParticles >= _maxParticles) {
        return -1;
    }
    
    return _aliveParticles++;
}

void ParticleSystem::killParticle(int particleID)
{
    _aliveParticles--;
    
    for (ParticleAttribute *attribute : _allParticleAttributes) {
        attribute->moveParticle(_aliveParticles, particleID);
    }
}

void ParticleSystem::update(float dt)
{
    for (int i = 0; i < _aliveParticles; ++i) {
        updateParticle(i, dt);
    }
    
    updateGeneral(dt);
}

/**
 * Default implementation for update general. May be overridden
 * by a particle system implementation to update the system
 */
void ParticleSystem::updateGeneral(float dt) {}

void ParticleSystem::render()
{
    if (_aliveParticles > 0) {
        for (ShaderAttribute *attribute : _shaderAttributes) {
            attribute->pushData();
        }
        
        preRender();
        _renderComponent->render(_worldTransform, _aliveParticles);
        postRender();
    }
}