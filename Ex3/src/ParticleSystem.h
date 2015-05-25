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
    std::string _name;
public:
    ParticleAttribute(int maxParticles, std::string name) :
    _maxParticles(maxParticles), _name(name) {}
    virtual ~ParticleAttribute() {}
    std::string getName()
    {
        return _name;
    }
    
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
    ParticleAttributeDerived(int maxParticles, std::string name) :
    ParticleAttribute(maxParticles, name),
    _values(maxParticles) {}
    
    virtual ~ParticleAttributeDerived() {}
    
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
    ParticleAttributeDerived<T>(maxParticles, variableName),
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

/* Some standard typedefed attributes */
typedef ShaderAttributeDerived<vec3, 3, GL_FLOAT> PositionAttribute;
typedef ShaderAttributeDerived<float, 1, GL_FLOAT> SizeAttribute;
typedef ShaderAttributeDerived<float, 1, GL_FLOAT> TransparencyAttribute;
typedef ShaderAttributeDerived<vec3, 3, GL_FLOAT> TintAttribute;
typedef ShaderAttributeDerived<vec2, 2, GL_FLOAT> BillboardRightAttribute;


/**
 * The particle system class. Handles lists of particle attributes
 */
class ParticleSystem : public RenderableSceneNode {
private:
    std::vector<ParticleAttribute*> _allParticleAttributes;
    std::vector<ShaderAttribute*> _shaderAttributes;
    
protected:
    unsigned int _maxParticles;
    unsigned int _aliveParticles; // Doubles as the next place in the array for a particle
    
    // Default attributes (they are part of the shader)
    PositionAttribute _positions;
    SizeAttribute _sizes;
    TransparencyAttribute _transparencies;
    TintAttribute _tints;
    BillboardRightAttribute _billboardRights;
    
    void addAttribute(ParticleAttribute *attribute);
    void addShaderAttribute(ShaderAttribute *attribute);

    int createNewParticle();
    void killParticle(unsigned int particleID);
    
    virtual void updateGeneral(float dt);
    virtual void updateParticle(unsigned int particleID, float dt) = 0;
    
    virtual void preRender();
public:
    ParticleSystem(unsigned int maxParticles,
                   vec3 position = vec3(0.0f),
                   quat rotation = quat(vec3(0.0f)),
                   vec3 scale = vec3(1.0f));
    virtual ~ParticleSystem();
    
    virtual void emit() = 0;
    
    virtual void update(float dt);
    virtual void render();
};

#endif /* defined(__CGP_Ex3__ParticleSystem__) */
