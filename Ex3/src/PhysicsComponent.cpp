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

PhysicsComponent::PhysicsComponent(float maxVelocity,
                                   float maxAcceleration,
                                   float dampeningSpeed) :
_velocity(0.0f), _acceleration(0.0f),
_maxVelocity(maxVelocity), _maxAcceleration(maxAcceleration),
_dampeningSpeed(dampeningSpeed)
{
}

PhysicsComponent::~PhysicsComponent(){}

vec3 normalizePower(vec3 force, float maxPower)
{
    vec3 ret = force;

    float power = length(force);
    if (power > maxPower) {
        ret = normalize(ret) * maxPower;
    }
    
    return ret;
}

void PhysicsComponent::update(float dt)
{
    _velocity += _acceleration;
    
    if (length(_velocity) > _dampeningSpeed && _acceleration != vec3(0.0f)) {
        vec3 dampening = normalize(_velocity) * _dampeningSpeed;
        _velocity -= dampening;
    }

    _velocity = normalizePower(_velocity, _maxVelocity);
    
    _acceleration = vec3(0.0f);
}

void PhysicsComponent::applyForce(vec3 force)
{
    // Update acceleration
    _acceleration += force;
    
    _acceleration = normalizePower(_acceleration, _maxAcceleration);
}

vec3 PhysicsComponent::getVelocity()
{
    return _velocity;
}

void PhysicsComponent::stopMoving()
{
    _acceleration = vec3(0.0f);
    _velocity = vec3(0.0f);
}