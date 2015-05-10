//
//  Ship.cpp
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ShaderIO.h"
#include "Ship.h"
#include "InputManager.h"

#include <iostream>

static const std::string SHIP_MESH = "assets/vipermk2.lwo";

#define TURN_SPEED (pi<float>() / 2.0f)
#define ACCELERATION_FORCE (0.003f)
#define DAMPENING_SPEED    (0.01f)
#define MAX_VELOCITY (3.0f)
#define MAX_HEALTH (5)

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
_forward(FORWARD_VECTOR), _right(RIGHT_VECTOR),
_radius(radius)
{
    // Initialize mesh
    {
        vec3 meshPosition = vec3(0.0f);
        quat meshRotation = rotate(quat(vec3(0.0f)), 90.0f, vec3(0.0f, 1.0f, 0.0f));
        vec3 meshScale = vec3(1.0f);
        _mesh = new Mesh(SHIP_MESH, meshPosition, meshRotation, meshScale);
        addChild(_mesh);
    }
    
    // Construct healthbar
    {
        vec3 healthPosition = vec3(0.0f, 1.0f, 0.0f);
        quat healthRotation = quat(vec3(0.0f));
        vec3 healthScale = vec3(1.0f, 0.1f, 1.0f);
        _healthBar = new HealthBar(MAX_HEALTH, healthPosition, healthRotation, healthScale);
        addChild(_healthBar);
    }
    
    // Initialize components
    _physicsComponent = new PhysicsComponent(MAX_VELOCITY);
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
    delete _physicsComponent;
    delete _healthBar;
}

void Ship::render()
{
}

void Ship::update(float dt)
{
    InputManager &input = InputManager::Instance();
    
    // Accelerating
    if (input.isPressed(KEY_ACTION)) {
        _physicsComponent->applyForce(_forward * ACCELERATION_FORCE);
    } else if (_physicsComponent->isMoving()) {
        _physicsComponent->applyForce(-_physicsComponent->getVelocity() * DAMPENING_SPEED);
    }
    
    // Tilting
    if (input.isPressed(KEY_UP))
    {
        tilt(TURN_SPEED * dt);
    }
    else if (input.isPressed(KEY_DOWN))
    {
        tilt(-TURN_SPEED * dt);
    }
    
    // Twisting
    if (input.isPressed(KEY_LEFT))
    {
        twist(-TURN_SPEED * dt);
    }
    else if (input.isPressed(KEY_RIGHT))
    {
        twist(TURN_SPEED * dt);
    }
    
    // Update position
    vec3 velocity = _physicsComponent->getVelocity();
    setPosition(_position + velocity * dt);
}

/**
 * Accelerate in the _forward direction
 */
void Ship::accelerate(float force)
{
    _physicsComponent->applyForce(_forward * force);
}

/**
 * Tilt ship up/down
 */
void Ship::tilt(float angle)
{
    setRotation(rotate(_rotation, angle, RIGHT_VECTOR));
    
    // No need to recompute right vector, as it didn't change.
    _forward = _rotation * FORWARD_VECTOR;
}

/**
 * Twist ship right/left
 */
void Ship::twist(float angle)
{
    setRotation(rotate(_rotation, angle, FORWARD_VECTOR));
    
    // No need to recompute forward vector, as it didn't change.
    _right = _rotation * RIGHT_VECTOR;
}

void Ship::collide()
{
//    std::cout << "Hit!" << std::endl;
    int health = _healthBar->getCurrentUnits();
    int newHealth = health - 1;
    
    if (newHealth >= 0) {
        _healthBar->setCurrentUnits(newHealth);
        if (newHealth == 0) {
            std::cout << "Game Over!" << std::endl;
        }
    }
}

/**
 * Return the forwar-facing vector
 */
vec3 Ship::getForward()
{
    return _forward;
}

/**
 * Return the right-facing vector
 */
vec3 Ship::getRight()
{
    return _right;
}

float Ship::getRadius()
{
    return _radius;
}
