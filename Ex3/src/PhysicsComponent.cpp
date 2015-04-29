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

void PhysicsComponent::update(float dt)
{
    _velocity += _acceleration;
    
    if (length(_velocity) > _dampeningSpeed && _acceleration != vec3(0.0f)) {
        vec3 dampening = normalize(_velocity) * _dampeningSpeed;
        _velocity -= dampening;
    }

    _velocity.x = clamp(_velocity.x, -_maxVelocity, _maxVelocity);
    _velocity.y = clamp(_velocity.y, -_maxVelocity, _maxVelocity);
    _velocity.z = clamp(_velocity.z, -_maxVelocity, _maxVelocity);
    
    _acceleration = vec3(0.0f);
}

void PhysicsComponent::applyForce(vec3 force)
{
    // Update acceleration
    _acceleration += force;
    
    _acceleration.x = min(_acceleration.x, _maxAcceleration);
    _acceleration.y = min(_acceleration.y, _maxAcceleration);
    _acceleration.z = min(_acceleration.z, _maxAcceleration);
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