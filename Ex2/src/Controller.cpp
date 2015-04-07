//
//  Controller.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 3/17/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Controller.h"
#include <iostream>

#define PLAYER_HEIGHT (2.0f)
#define PLAYER_CROUCH_HEIGHT (0.5f)
#define MOVE_SPEED (0.2f)
#define MOVE_BOOST (2.5);
#define CROUCH_SPEED (0.4)
#define TURN_ANGLE (10.0f)
#define LEAN_ANGLE (5.0f)
#define JUMP_SPEED (1.5f)
#define GRAVITY (0.4f)

/**
 * Constructor. Sets camera to the desired initial position.
 */
Controller::Controller(Camera *camera, World *world) :
_myCamera(camera),
_world(world),
_isJumping(false),
_yVelocity(0),
_isCrouched(false)
{
    // Set camera to middle of corridor
    moveToPosition(world->getStartPosition());
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
//        vec3 movementVec = _myCamera->getDir() * MOVE_SPEED;
//        move(movementVec * dt);
        
        _world->advanceCorridor(-MOVE_SPEED * dt);
    }
    else if (input.isPressed(KEY_BACKWARD))
    {
//        vec3 movementVec = -_myCamera->getDir() * MOVE_SPEED;
//        move(movementVec * dt);
        
        _world->advanceCorridor(MOVE_SPEED * dt);
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
 * Move in a direction
 */
void Controller::move(vec3 direction)
{
    // Check for boost;
    if (InputManager::Instance().isModifierPressed())
    {
        direction *= MOVE_BOOST;
    }
    
    vec3 newPos = _myCamera->getPos() + direction;
    newPos.y = 0.0f;
    
    moveToPosition(newPos);
}

/**
 * Move in a direction
 */
void Controller::moveToPosition(vec3 position)
{
    float playerHeight = (_isCrouched) ? PLAYER_CROUCH_HEIGHT : PLAYER_HEIGHT;
    position.y += playerHeight;
    
    // Set position
    _myCamera->setPos(position);
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

