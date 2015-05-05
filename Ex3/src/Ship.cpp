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

static const std::string SHIP_TEXTURE = "assets/wallTexture-squashed.bmp";
static const std::string SHIP_BUMP = "assets/wallTexture-squashed-bump.bmp";

#define TURN_SPEED (pi<float>() / 2)
#define ACCELERATION_FORCE (0.001f)
#define DAMPENING_FORCE    (0.0001f)
#define MAX_ACCELERATION (0.1)
#define MAX_VELOCITY (0.1)
#define EPS (0.0001)

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, quat rotation, vec3 scale) :
RenderableSceneNode("ShipShader", position, rotation, scale),
_forward(rotation * FORWARD_VECTOR), _right(rotation * RIGHT_VECTOR)
{
    // Initialize components
    _physicsComponent = new PhysicsComponent(MAX_VELOCITY, MAX_ACCELERATION, DAMPENING_FORCE);
    
    // Initialize ship
    static const GLfloat vertices[] = {
        // Left wall
        -0.5f, -0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f,
        
        -0.5f, -0.5f, -0.5f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f,
        
        // Right wall
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f
    };


    // Push VBO
    std::vector<GLfloat> verticesVector(vertices, vertices + (sizeof(vertices) / sizeof(GLfloat)));
    _renderComponent->setVBO(verticesVector);
    
    static const GLubyte indices[] = {
        0, 1, 2,
        1, 3, 2,
        1, 5, 3,
        3, 7, 5,
        5, 4, 7,
        4, 6, 7,
        4, 0, 6,
        0, 2, 6
    };
    
    std::vector<GLubyte> indicesVector(indices, indices + (sizeof(indices) / sizeof(GLubyte)));
    _renderComponent->setIBO(indicesVector);
    
    // Create ship textures
    {
        _renderComponent->add2DTexture(SHIP_TEXTURE);
        _renderComponent->add2DTexture(SHIP_BUMP);
    }
    
    // Construct healthbar
    {
        vec3 healthPosition = position + vec3(0.0f, 1.0f, 0.0f);
        quat healthRotation = quat(vec3(0.0f));
        vec3 healthScale = vec3(1.0f, 0.1f, 1.0f);
        _healthBar = new HealthBar(5, healthPosition, healthRotation, healthScale);
        addChild(_healthBar);
        
        _healthBar->setCurrentUnits(3);
    }
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
    delete _physicsComponent;
    delete _healthBar;
}

void Ship::update(float dt)
{
    InputManager &input = InputManager::Instance();
    
    // Accelerating
    if (input.isPressed(KEY_ACTION)) {
        accelerate(ACCELERATION_FORCE);
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
    
    // Update physics
    _physicsComponent->update(dt);
    
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