//
//  AsteroidRigidBody.h
//  CGP-Ex4
//
//  Created by Amir Blum on 6/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__AsteroidRigidBody__
#define __CGP_Ex4__AsteroidRigidBody__

#include <stdio.h>
#include "RigidBody.h"

class AsteroidRigidBody : public RigidBody {
public:
    AsteroidRigidBody(vec3 initialPos, vec3 initialVel, float radius, float mass);
    virtual ~AsteroidRigidBody();
    virtual void onCollision(RigidBody *other);
};

#endif /* defined(__CGP_Ex4__AsteroidRigidBody__) */
