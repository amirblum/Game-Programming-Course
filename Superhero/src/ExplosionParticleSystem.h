//
//  ExplosionParticleSystem.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/11/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__ExplosionParticleSystem__
#define __CGP_Superhero__ExplosionParticleSystem__

#include <stdio.h>
#include "ParticleSystem.h"

typedef ParticleAttributeDerived<vec3> VelocityAttribute;

class ExplosionParticleSystem : public ParticleSystem {
private:
    VelocityAttribute _velocities;
    float _nextEmitTime;
    float _timeSinceLastEmission;
    unsigned int _particlesEmitted;
protected:
    virtual void updateGeneral(float dt);
    virtual void updateParticle(unsigned int particleID, float dt);
public:
    ExplosionParticleSystem(unsigned int maxParticles,
                            vec3 position = vec3(0.0f),
                            quat rotation = quat(vec3(0.0f)),
                            vec3 scale = vec3(1.0f));
    virtual ~ExplosionParticleSystem();
    
    virtual void emit();
    bool isDead();
};

#endif /* defined(__CGP_Superhero__ExplosionParticleSystem__) */
