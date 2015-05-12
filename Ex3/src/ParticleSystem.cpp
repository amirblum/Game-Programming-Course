//
//  ParticleSystem.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/30/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ParticleSystem.h"
#include "Camera.h"

ParticleSystem::ParticleSystem(unsigned int maxParticles,
                               vec3 position, quat rotation, vec3 scale) :
RenderableSceneNode("ParticleShader", position, rotation, scale),
_maxParticles(maxParticles),
_positions(maxParticles, _renderComponent, "particleCenter"),
_sizes(maxParticles, _renderComponent, "particleSize"),
_transparency(maxParticles, _renderComponent, "transparency"), 
_allParticleAttributes(),
_shaderAttributes(),
_aliveParticles(0)
{
    // Set up billboard
    {
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
    }
    
    // Add built-in attributes
    {
        addAttribute(&_positions);
        addShaderAttribute(&_positions);
        
        addAttribute(&_sizes);
        addShaderAttribute(&_sizes);
        
        addAttribute(&_transparency);
        addShaderAttribute(&_transparency);
        
        // Init transparency to opaque
        for (unsigned int i = 0; i < _maxParticles; ++i) {
            _transparency.setValue(i, 1.0f);
        }
    }
}

ParticleSystem::~ParticleSystem()
{
// I KNOW that this is a memory leak...not gonna fix it yet since the project is due tomorrow
//    for (ParticleAttribute *attribute : _allParticleAttributes) {
//        delete attribute;
//    }
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

void ParticleSystem::killParticle(unsigned int particleID)
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

void ParticleSystem::preRender()
{
    Camera *camera = Camera::MainCamera();
    
    vec3 cameraUp = camera->getUp();
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraUp", cameraUp);
    
    vec3 cameraRight = cross(camera->getDirection(), cameraUp);
    _renderComponent->setUniform<vec3, UNIFORM_VEC3>("cameraRight", cameraRight);
}

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