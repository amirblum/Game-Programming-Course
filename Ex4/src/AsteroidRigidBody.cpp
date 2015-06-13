//
//  AsteroidRigidBody.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "AsteroidRigidBody.h"

AsteroidRigidBody::AsteroidRigidBody(vec3 initialPos, vec3 initialVel, float radius, float mass) :
RigidBody(initialPos, initialVel, radius, mass, false) {}

AsteroidRigidBody::~AsteroidRigidBody() {}

void AsteroidRigidBody::onCollision(RigidBody *other)
{
    
}