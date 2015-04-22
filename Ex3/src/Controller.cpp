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

#define PLAYER_HEIGHT (2.0f)
#define PLAYER_CROUCH_HEIGHT (0.5f)

#define LIGHT_OFFSET_X (-0.5f)
#define LIGHT_OFFSET_Y (0.0f)
#define LIGHT_OFFSET_Z (0.0f)

#define AVERAGE_LIGHT_FLICKER_TIME (3.0f)
#define RANGE_LIGHT_FLICKER_TIME (2.0f)
#define JUMP_SCARE_TIME (7.0f)

#define MOVE_SPEED (0.02f)
#define STRAFE_SPEED (0.2f)

#define BOB_SPEED (0.01f)
#define BOB_MAX (0.1f)

#define MOVE_BOOST (2.5);

#define TURN_ANGLE (10.0f)
#define LEAN_ANGLE (5.0f)

/**
 * Constructor. Sets camera to the desired initial position.
 */
Controller::Controller(Camera *camera, World *world) :
_myCamera(camera),
_world(world)
{
    vec3 startPosition = world->getStartPosition();
    vec3 cameraPosition = vec3(startPosition.x, startPosition.y + PLAYER_HEIGHT, startPosition.z);
    
    // Set camera to middle of ship
    _myCamera->setPos(cameraPosition);
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
        vec3 movementVec = _myCamera->getDir() * MOVE_SPEED;
        move(movementVec * dt);
    }
    else if (input.isPressed(KEY_BACKWARD))
    {
        vec3 movementVec = -_myCamera->getDir() * MOVE_SPEED;
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
        move(-cross(_myCamera->getDir(), _myCamera->getUp()) * MOVE_SPEED * dt);
    }
    else if (input.isPressed(KEY_STRAFE_RIGHT))
    {
        move(cross(_myCamera->getDir(), _myCamera->getUp()) * MOVE_SPEED * dt);
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
    
    vec3 newPos = _myCamera->getPos() + direction;
    newPos.y = PLAYER_HEIGHT;
    
    
    // Set position
    _myCamera->setPos(newPos);
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
    
    vec3 newCameraPos = _myCamera->getPos() + direction;
    
    _myCamera->setPos(newCameraPos);
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
    vec4 newDir(_myCamera->getDir(), 1.0f);
    newDir = rotate(mat4(1.0f), angle, _myCamera->getUp()) * newDir;
    _myCamera->setDir(normalize(vec3(newDir.x, newDir.y, newDir.z)));
}

