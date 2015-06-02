//
//  RigidBody.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "RigidBody.h"
#include "PhysicsSimulation.h"

RigidBody::RigidBody(vec3 initialPosition, float radius, float mass, bool isSource) :
_radius(radius), _physics(initialPosition, mass)
{
    PhysicsSimulation::Instance().addObject(this, isSource);
}

RigidBody::~RigidBody()
{
    PhysicsSimulation::Instance().removeObject(this);
}

void RigidBody::physicsUpdate(float dt)
{
    _physics.update(dt);
}

PhysicsComponent& RigidBody::getPhysics()
{
    return _physics;
}

const PhysicsComponent& RigidBody::getPhysics() const
{
    return _physics;
}

float RigidBody::getRadius() const
{
    return _radius;
}

void RigidBody::handleCollision(const RigidBody &collided, const vec3 &collisionNormal)
{
    _physics.handleCollision(collided.getPhysics(), collisionNormal);
    
    onCollision(collided);
}

void RigidBody::onCollision(const RigidBody &collided)
{
    std::cout << "Collided!" << std::endl;
}