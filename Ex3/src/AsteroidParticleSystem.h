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

typedef ShaderAttributeDerived<vec3, GL_FLOAT> PositionAttribute;

class AsteroidParticleSystem : public ParticleSystem {
private:
    float _asteroidRadius;
    float _emitRadius;
    Ship *_ship;
    
    PositionAttribute _positionAttribute;
    std::vector<PhysicsComponent*> _physicsComponents;
public:
    AsteroidParticleSystem(int maxAsteroids, float asteroidRadius, float radius, Ship *ship);
    virtual ~AsteroidParticleSystem();
    
    virtual void emit();
    virtual void updateParticle(int particleID, float dt);
};

#endif /* defined(__CGP_Ex3__AsteroidParticleSystem__) */
