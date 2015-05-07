//
//  ParticleSystem.h
//  CGP-Ex3
//
//  Particle system implementation
//
//  Created by Amir Blum on 4/30/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__ParticleSystem__
#define __CGP_Ex3__ParticleSystem__

#include <stdio.h>
#include <iostream>
#include <vector>
#include "RenderableSceneNode.h"

/**
 * A list of attributes
 */
class ParticleAttribute {
protected:
    int _maxParticles;
public:
    ParticleAttribute(int maxParticles) :
    _maxParticles(maxParticles) {}
    virtual ~ParticleAttribute() {}
    
    virtual void moveParticle(int fromID, int toID) = 0;
};

class ShaderAttribute {
protected:
    RenderComponent *_renderer;
    GLuint _attributeVBO;
public:
    ShaderAttribute(RenderComponent *renderer) :
    _renderer(renderer),
    _attributeVBO(0) {}
    
    virtual ~ShaderAttribute() {}
    
    virtual void pushData() = 0;
};

/**
 * The generic implementations of particle attributes
 */
template <class T>
class ParticleAttributeDerived : public ParticleAttribute {
protected:
    std::vector<T> _values;
public:
    ParticleAttributeDerived(int maxParticles) :
    ParticleAttribute(maxParticles),
    _values(maxParticles) {}
    
    virtual ~ParticleAttributeDerived() {}
    
//    T& operator[](unsigned int i)
//    {
//        return _values[i];
//    }
    
    T getValue(unsigned int i)
    {
        return _values[i];
    }
    
    void setValue(unsigned int i, T newValue)
    {
        _values[i] = newValue;
    }
    
    virtual void moveParticle(int fromID, int toID)
    {
        _values[toID] = _values[fromID];
    }
};

/**
 * The generic implementations of particle attributes
 */
template <class T, int size, GLenum oglType>
class ShaderAttributeDerived : public ParticleAttributeDerived<T>, public ShaderAttribute {
public:
    ShaderAttributeDerived(int maxParticles, RenderComponent *renderer, std::string variableName) :
    ParticleAttributeDerived<T>(maxParticles),
    ShaderAttribute(renderer)
    {
        _attributeVBO = _renderer->createSupportVBO(oglType, size, variableName, 1);
    }
    
    virtual ~ShaderAttributeDerived() {}
    
    virtual void pushData()
    {
        _renderer->updateSupportVBO<T>(_attributeVBO, ParticleAttributeDerived<T>::_values);
    }
};

/**
 * The particle system class. Handles lists of particle attributes
 */
class ParticleSystem : public RenderableSceneNode {
private:
    std::vector<ParticleAttribute*> _allParticleAttributes;
    std::vector<ShaderAttribute*> _shaderAttributes;
    int _aliveParticles; // Doubles as the next place in the array for a particle
    
protected:
    size_t _maxParticles;
    void addAttribute(ParticleAttribute *attribute);
    void addShaderAttribute(ShaderAttribute *attribute);
    
public:
    ParticleSystem(int maxParticles,
                   std::string shaderProgram,
                   vec3 position = vec3(0.0f),
                   quat rotation = quat(vec3(0.0f)),
                   vec3 scale = vec3(1.0f));
    virtual ~ParticleSystem();
    
    int createNewParticle();
    void killParticle(int particleID);
    virtual void emit() = 0;
    
    virtual void update(float dt);
    virtual void updateGeneral(float dt);
    virtual void updateParticle(int particleID, float dt) = 0;
    virtual void render();
};

#endif /* defined(__CGP_Ex3__ParticleSystem__) */
