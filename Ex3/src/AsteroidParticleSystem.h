//
//  AsteroidParticleSystem.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__AsteroidParticleSystem__
#define __CGP_Ex3__AsteroidParticleSystem__

#include <stdio.h>
#include "ParticleSystem.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "Ship.h"

typedef ShaderAttributeDerived<vec3, 3, GL_FLOAT> PositionAttribute;
typedef ShaderAttributeDerived<float, 1, GL_FLOAT> SizeAttribute;
typedef ParticleAttributeDerived<PhysicsComponent*> PhysicsAttribute;
typedef ParticleAttributeDerived<bool> CollisionAttribute;

class AsteroidParticleSystem : public ParticleSystem {
private:
    float _emitMaxRadius, _emitMinRadius;
    Ship *_ship;
    
    PositionAttribute _positions;
    SizeAttribute _sizes;
    PhysicsAttribute _physics;
    CollisionAttribute _collided;
    
    bool particleInView(vec3 particlePosition);
public:
    AsteroidParticleSystem(int maxAsteroids, float emitRadius, Ship *ship);
    virtual ~AsteroidParticleSystem();
    
    virtual void emit();
    virtual void updateParticle(int particleID, float dt);
    virtual void preRender();
};

#endif /* defined(__CGP_Ex3__AsteroidParticleSystem__) */
