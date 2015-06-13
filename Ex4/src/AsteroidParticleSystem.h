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
//#include "Ship.h"

typedef ParticleAttributeDerived<AsteroidRigidBody*> RigidBodyAttribute;

class AsteroidParticleSystem : public ParticleSystem {
private:
    float _emitMaxRadius, _emitMinRadius;
    
    RigidBodyAttribute _rigidBodies;
    
    bool particleInView(vec3 particlePosition);
protected:
    virtual void updateParticle(unsigned int particleID, float dt);
public:
    AsteroidParticleSystem(unsigned int maxAsteroids, float emitRadius);
    virtual ~AsteroidParticleSystem();
    
    virtual void emit();
};

#endif /* defined(__CGP_Ex4__AsteroidParticleSystem__) */