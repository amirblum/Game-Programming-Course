//
//  PhysicsComponent.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/28/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "PhysicsComponent.h"
#include <glm/gtx/rotate_vector.hpp>

#define EPS (0.0001f)

PhysicsComponent::PhysicsComponent(float maxVelocity) :
_velocity(0.0f), _acceleration(0.0f),
_maxVelocity(maxVelocity)
{
}

PhysicsComponent::~PhysicsComponent(){}

vec3 normalizePower(vec3 force, float maxPower)
{
    vec3 ret = force;

    float power = length(force);
    if (power > maxPower) {
        ret = normalize(ret) * maxPower;
    } else if (power < EPS) {
        ret = vec3(0.0f);
    }
    
    return ret;
}

void PhysicsComponent::applyForce(vec3 force)
{
    _velocity += force;
    _velocity = normalizePower(_velocity, _maxVelocity);
}

vec3 PhysicsComponent::getVelocity()
{
    return _velocity;
}

bool PhysicsComponent::isMoving()
{
    return length(_velocity) != 0;
}