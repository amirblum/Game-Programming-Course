//
//  AsteroidRigidBody.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "AsteroidRigidBody.h"

AsteroidRigidBody::AsteroidRigidBody(vec3 initialPos, vec3 initialVel, float radius, float mass) :
RigidBody(initialPos, initialVel, radius, mass, false),
_dead(false)
{}

AsteroidRigidBody::~AsteroidRigidBody() {}

void AsteroidRigidBody::onCollision(RigidBody *collided)
{
    // Black Hole
    BlackHole *blackHole = dynamic_cast<BlackHole*>(collided);
    if (blackHole != nullptr) {
        collideWithBlackHole(blackHole);
        return;
    }
    
    // Ship
    Ship *ship = dynamic_cast<Ship*>(collided);
    if (ship != nullptr) {
        collideWithShip(ship);
        return;
    }
}

void AsteroidRigidBody::collideWithBlackHole(BlackHole *blackHole)
{
    _dead = true;
}

void AsteroidRigidBody::collideWithShip(Ship *ship)
{
    if (ship->getSpeed() > ship->getMaxSpeed() / 2.0f) {
        _dead = true;
        // TODO generate asteroid explosion
    }
}

bool AsteroidRigidBody::isDead()
{
    bool ret = _dead;
    if (_dead) {
        _dead = false;
    }
    return ret;
}