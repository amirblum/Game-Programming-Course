//
//  PhysicsComponent.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/28/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "PhysicsComponent.h"
#include <glm/gtx/rotate_vector.hpp>

PhysicsComponent::PhysicsComponent(float maxVelocity, float maxAcceleration) :
_velocity(0.0f), _acceleration(0.0f), _maxVelocity(maxVelocity), _maxAcceleration(maxAcceleration)
{
}

PhysicsComponent::~PhysicsComponent(){}

void PhysicsComponent::update(float dt)
{
    _velocity += _acceleration;
    
    _velocity.x = min(_velocity.x, _maxVelocity);
    _velocity.y = min(_velocity.y, _maxVelocity);
    _velocity.z = min(_velocity.z, _maxVelocity);
    
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