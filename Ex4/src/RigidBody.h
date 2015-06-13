//
//  RigidBody.h
//  CGP-Ex4
//
//  A RigidBody is an object with a physics component and a radius.
//  This assumes all objects in our world are spheres (handy)
//
//  Created by Amir Blum on 6/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__RigidBody__
#define __CGP_Ex4__RigidBody__

#include <iostream>
#include "PhysicsComponent.h"

class RigidBody {
protected:
    float _radius;
    PhysicsComponent _physics;
    
public:
    RigidBody(vec3 initialPos, vec3 initialVel, float radius, float mass, bool isSource);
    virtual ~RigidBody();
    
    void physicsUpdate(float dt);
    
    PhysicsComponent& getPhysics();
    const PhysicsComponent& getPhysics() const;
    float getRadius() const;
    void handleCollision(RigidBody *collided, const vec3 &collisionNormal);
    virtual void onCollision(RigidBody *collided);
};

#endif /* defined(__CGP_Ex4__RigidBody__) */
