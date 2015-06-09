//
//  PhysicsComponent.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 4/28/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "PhysicsComponent.h"

#define EPS (0.0001f)
#define G (6.674f)
#define COLLISION_ELASTICITY (0.5f)

PhysicsComponent::PhysicsComponent(vec3 initialPos, vec3 initialVel, float mass) :
_mass(mass), _inverseMass(1.0f / mass), _isStatic(false),
_internalForce(0.0f)
{
    _currentState = State{initialPos, initialVel};
    _previousState = _currentState;
}

PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::applyForce(vec3 force)
{
    _internalForce += force;
}

void PhysicsComponent::addForceSource(PhysicsComponent *source)
{
    _externalForceSources.push_back(source);
}

void PhysicsComponent::removeForceSource(PhysicsComponent *source)
{
    // Remove from objects vector
    auto iter = std::find(_externalForceSources.begin(), _externalForceSources.end(), source);
    if (iter != _externalForceSources.end()) {
        _externalForceSources.erase(iter);
    }
}

void PhysicsComponent::setStatic(bool isStatic)
{
    _isStatic = isStatic;
}

void PhysicsComponent::update(float dt)
{
    _previousState = _currentState;
    rungeKutta4(dt);
    _internalForce = vec3(0.0f);
}

PhysicsComponent::Derivative PhysicsComponent::evaluate(float dt, const Derivative &d)
{
    State state;
    state.x = _currentState.x + d.dx * dt;
    state.v = _currentState.v + d.dv * dt;
    
    Derivative output;
    output.dx = state.v;
    output.dv = acceleration(state);
    return output;
}

vec3 PhysicsComponent::acceleration(const State &state)
{
    // Initialize with internal force
    vec3 forceVec(_internalForce);
    
    if (!_isStatic) {
        // Then add the external forces
        for (PhysicsComponent *externalSource : _externalForceSources) {
            vec3 vecToSource = externalSource->getPosition() - state.x;
            float r = length(vecToSource);
            float otherMass = externalSource->getMass();
            float gforce = G * (_mass * otherMass) / (r * r);
            forceVec += gforce * normalize(vecToSource);
        }
    }
    
    return forceVec;
}

void PhysicsComponent::rungeKutta4(float dt)
{
    Derivative a,b,c,d;
    
    a = evaluate(0.0f, Derivative());
    b = evaluate(dt * 0.5f, a);
    c = evaluate(dt * 0.5f, b);
    d = evaluate(dt, c);
    
    vec3 dxdt = 1.0f / 6.0f * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
    
    vec3 dvdt = 1.0f / 6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);
    
    _currentState.x = _currentState.x + dxdt * dt;
    _currentState.v = _currentState.v + dvdt * dt;
}

vec3 PhysicsComponent::getPosition() const
{
    return _currentState.x;
}

vec3 PhysicsComponent::getPreviousPosition() const
{
    return _previousState.x;
}

vec3 PhysicsComponent::getVelocity() const
{
    return _currentState.v;
}

float PhysicsComponent::getMass() const
{
    return _mass;
}

float PhysicsComponent::getInverseMass() const
{
    return _inverseMass;
}

bool PhysicsComponent::isMoving() const
{
    return length(_currentState.v) != 0;
}

void PhysicsComponent::handleCollision(const PhysicsComponent &other, const vec3 &collisionNormal)
{
    if (_isStatic) {
        return;   
    }
    
    vec3 dv = getVelocity() - other.getVelocity();
    
    float jNumerator = -(1.0f + COLLISION_ELASTICITY) * dot(dv, collisionNormal);
    float jDenominator = (_inverseMass + other._inverseMass) * dot(collisionNormal, collisionNormal);
    
    float j = jNumerator / jDenominator;
    
    _currentState.v = _currentState.v + (j / _mass) * collisionNormal;
}