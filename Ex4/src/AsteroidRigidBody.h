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
#include "BlackHole.h"
#include "Ship.h"

class AsteroidRigidBody : public RigidBody {
private:
    bool _dead;
    void collideWithBlackHole(BlackHole *blackHole);
    void collideWithShip(Ship *ship);
    
protected:
    virtual void onCollision(RigidBody *collided);
    
public:
    AsteroidRigidBody(vec3 initialPos, vec3 initialVel, float radius, float mass);
    virtual ~AsteroidRigidBody();
    
    bool isDead();
};

#endif /* defined(__CGP_Ex4__AsteroidRigidBody__) */
