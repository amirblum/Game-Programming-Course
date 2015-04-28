//
//  Ship.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Ship.h"
#include "InputManager.h"

#include <iostream>

static const std::string SHIP_TEXTURE = "assets/wallTexture-squashed.bmp";
static const std::string SHIP_BUMP = "assets/wallTexture-squashed-bump.bmp";

#define TURN_SPEED (2.5f)
#define ACCELERATION_FORCE (0.1f)

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, quat rotation, vec3 scale) :
SceneNode(position, rotation, scale),
_renderComponent("ship", "ShipShader.vert", "ShipShader.frag"),
_physicsComponent(0.1, 0.1),
_forward(rotation * FORWARD_VECTOR), _up(rotation * UP_VECTOR)
{
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
    _renderComponent.setVBO(verticesVector);
    
    
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
    _renderComponent.setIBO(indicesVector);
    
    // Create ship textures
    {
        _renderComponent.addTexture(SHIP_TEXTURE, GL_TEXTURE_2D);
        _renderComponent.addTexture(SHIP_BUMP, GL_TEXTURE_2D);
    }
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
}

void Ship::update(float dt)
{
    InputManager &input = InputManager::Instance();
    
    // Accelerating
    if (input.isPressed(KEY_ACTION)) {
        accelerate(ACCELERATION_FORCE * dt);
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
    
    // Position in space
    _physicsComponent.update(dt);
    
    vec3 newPosition = _position + _physicsComponent.getVelocity() * dt;
    setPosition(newPosition);
}

void Ship::render()
{
    _renderComponent.render(_worldTransform);
}

void Ship::accelerate(float force)
{
    _physicsComponent.applyForce(_forward * force);
}

void Ship::tilt(float angle)
{
    vec3 xAxis = normalize(cross(_up, _forward));
    setRotation(rotate(_rotation, angle, xAxis));
    
    _forward = _rotation * FORWARD_VECTOR;
    _up = _rotation * UP_VECTOR;
    
}

void Ship::twist(float angle)
{
    setRotation(rotate(_rotation, angle, _forward));
    
    // No need to recompute forward vector, as it didn't change.
    _up = _rotation * UP_VECTOR;
}

vec3 Ship::getForward()
{
    return _forward;
}

vec3 Ship::getUp()
{
    return _up;
}