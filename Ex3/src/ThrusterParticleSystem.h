//
//  ThrusterParticleSystem.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__ThrusterParticleSystem__
#define __CGP_Ex3__ThrusterParticleSystem__

#include <stdio.h>
#include "ParticleSystem.h"

class ThrusterParticleSystem : public ParticleSystem {
private:
    float _thrusterRadius;
    ParticleAttributeDerived<vec2> _originalDistances;
    
    float _nextEmitTime;
    float _timeSinceLastEmission;
protected:
    virtual void updateGeneral(float dt);
    virtual void updateParticle(unsigned int particleID, float dt);
public:
    ThrusterParticleSystem(unsigned int maxParticles,
                           float thrusterRadius,
                           vec3 position = vec3(0.0f),
                           quat rotation = quat(vec3(0.0f)),
                           vec3 scale = vec3(1.0f));
    virtual ~ThrusterParticleSystem();
    
    virtual void emit();
};

#endif /* defined(__CGP_Ex3__ThrusterParticleSystem__) */
