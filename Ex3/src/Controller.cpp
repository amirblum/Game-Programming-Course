//
//  Controller.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 3/17/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Controller.h"
#include "InputManager.h"

#include <iostream>

#define MOVE_SPEED (0.3f)
#define MOVE_BOOST (2.5)

#define TURN_ANGLE (10.0f)
#define LEAN_ANGLE (5.0f)

/**
 * Constructor. Sets camera to the desired initial position.
 */
Controller::Controller(Ship *ship, vec3 startPosition) :
_ship(ship)
{
    // Set ship to starting position
    _ship->setPosition(startPosition);
}

/**
 * The update function. Every frame check inputs and move camera around accordingly.
 */
void Controller::update(float dt)
{
    InputManager &input = InputManager::Instance();
    
    // Moving
    if (input.isPressed(KEY_FORWARD))
    {
        vec3 movementVec = _ship->getRotation() * MOVE_SPEED;
        move(movementVec * dt);
    }
    else if (input.isPressed(KEY_BACKWARD))
    {
        vec3 movementVec = -_ship->getRotation() * MOVE_SPEED;
        move(movementVec * dt);
    }
    
    // Turning
    if (input.isPressed(KEY_TURN_LEFT))
    {
        float angle = TURN_ANGLE;
        turn(angle * dt);
    }
    else if (input.isPressed(KEY_TURN_RIGHT))
    {
        float angle = -TURN_ANGLE;
        turn(angle * dt);
    }
    
    // Strafing
    if (input.isPressed(KEY_STRAFE_LEFT))
    {
        move(-cross(_ship->getRotation(), Camera::MainCamera()->getUp()) * MOVE_SPEED * dt);
    }
    else if (input.isPressed(KEY_STRAFE_RIGHT))
    {
        move(cross(_ship->getRotation(), Camera::MainCamera()->getUp()) * MOVE_SPEED * dt);
    }
}

/**
 * Advance down the ship
 */
void Controller::move(vec3 direction)
{
    // Check for boost;
    if (InputManager::Instance().isModifierPressed())
    {
        direction *= MOVE_BOOST;
    }
    
    vec3 newPos = _ship->getPosition() + direction;
    
    
    // Set position
    _ship->setPosition(newPos);
}

/**
 * Move in a direction
 */
void Controller::strafe(vec3 direction)
{
    // Check for boost;
    if (InputManager::Instance().isModifierPressed())
    {
        direction *= MOVE_BOOST;
    }
    
    vec3 newCameraPos = _ship->getPosition() + direction;
    
    _ship->setPosition(newCameraPos);
}

/**
 * Turn the camera
 */
void Controller::turn(float angle)
{
    // Check for boost
    if (InputManager::Instance().isModifierPressed())
    {
        angle *= MOVE_BOOST;
    }
    
    // Probably not the prettiest, but use a glm rotation matrix to rotate
    // to the desired direction
    vec4 newDir(_ship->getRotation(), 1.0f);
    newDir = rotate(mat4(1.0f), angle, Camera::MainCamera()->getUp()) * newDir;
    _ship->setRotation(normalize(vec3(newDir.x, newDir.y, newDir.z)));
}

