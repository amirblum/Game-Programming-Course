//
//  AsteroidParticleSystem.h
//  CGP-Ex4
//
//  Created by Amir Blum on 5/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__AsteroidParticleSystem__
#define __CGP_Ex4__AsteroidParticleSystem__

#include <stdio.h>
#include "ParticleSystem.h"
#include "RenderComponent.h"
#include "AsteroidRigidBody.h"
#include "PhysicsSimulation.h"

typedef ParticleAttributeDerived<AsteroidRigidBody*> RigidBodyAttribute;

class AsteroidParticleSystem : public ParticleSystem {
private:
    vec3 _center;
    mat4 _axisRotation;
    float _emitMaxRadius, _emitMinRadius;
    
    RigidBodyAttribute _rigidBodies;
    
    bool particleInView(vec3 particlePosition);
    
    vec3 asteroidToWorldPosition(vec3 asteroidPosition);
    vec3 worldToAsteroidPosition(vec3 worldPosition);
    vec3 rotateAxis(vec3 vector);
protected:
    virtual void updateParticle(unsigned int particleID, float dt);
public:
    AsteroidParticleSystem(vec3 center, quat axisRotation, unsigned int maxAsteroids, float emitRadius);
    virtual ~AsteroidParticleSystem();
    
    virtual void emit();
};

#endif /* defined(__CGP_Ex4__AsteroidParticleSystem__) */