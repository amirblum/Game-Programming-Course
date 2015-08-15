//
//  Controller.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/17/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Controller.h"
#include <iostream>

#define PLAYER_HEIGHT (1.0f)
#define PLAYER_CROUCH_HEIGHT (0.5f)
#define MOVE_SPEED (4.0f)
#define MOVE_BOOST (5.5);
#define CROUCH_SPEED (0.4)
#define TURN_ANGLE (5.0f)
#define LEAN_ANGLE (5.0f)
#define JUMP_SPEED (1.5f)
#define GRAVITY (0.4f)

/**
 * Constructor. Sets camera to the desired initial position.
 */
Controller::Controller(Camera *camera) :
_myCamera(camera),
_isJumping(false),
_yVelocity(0),
_isCrouched(false)
{
    // Set camera to corner of terrain to begin
    _myCamera->setPosition(vec3(0.0f, PLAYER_HEIGHT, 0.0f));
}

Controller::~Controller() { }

/**
 * The update function. Every frame check inputs and move camera around accordingly.
 */
void Controller::update(float dt)
{
    InputManager &input = InputManager::Instance();
    
    // Moving
    if (input.isPressed(KEY_UP))
    {
        vec3 movementVec = _myCamera->getDirection() * MOVE_SPEED;
        move(movementVec * dt);
    }
    else if (input.isPressed(KEY_DOWN))
    {
        vec3 movementVec = -_myCamera->getDirection() * MOVE_SPEED;
        move(movementVec * dt);
    }
    
    // Turning
    if (input.isPressed(KEY_LEFT))
    {
        float angle = TURN_ANGLE;
        turn(angle * dt);
    }
    else if (input.isPressed(KEY_RIGHT))
    {
        float angle = -TURN_ANGLE;
        turn(angle * dt);
    }
    
    // Strafing
    if (input.isPressed(KEY_LEFT_MOD))
    {
        move(-cross(_myCamera->getDirection(), _myCamera->getUp()) * MOVE_SPEED * dt);
    }
    else if (input.isPressed(KEY_RIGHT_MOD))
    {
        move(cross(_myCamera->getDirection(), _myCamera->getUp()) * MOVE_SPEED * dt);
    }
    
    // Jumping / crouching
    
    // Cache couch press
    bool crouchPressed = input.isPressedFirstTime(KEY_ACTION_MOD);
    
    if (_isJumping)
    {
        fall(dt);
    }
    else if (input.isPressedFirstTime(KEY_ACTION) && !_isCrouched)
    {
        jump();
    }
    else if ((!_isCrouched && crouchPressed) || (!_isCrouched && _isCrouching))
    {
        _isCrouching = true;
        crouch(dt);
    }
    else if ((_isCrouched && crouchPressed) || (_isCrouched && _isCrouching))
    {
        _isCrouching = true;
        uncrouch(dt);
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
    
    vec3 newPos = _myCamera->getPosition() + direction;
    
    // Set height appropriately
    if (!_isJumping && !_isCrouching)
    {
        float playerHeight = (_isCrouched) ? PLAYER_CROUCH_HEIGHT : PLAYER_HEIGHT;
        newPos.y = playerHeight;
    }
    
    // Set position
    _myCamera->setPosition(newPos);
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
    vec4 newDir(_myCamera->getDirection(), 1.0f);
    newDir = rotate(mat4(1.0f), angle, _myCamera->getUp()) * newDir;
    _myCamera->setDirection(normalize(vec3(newDir.x, newDir.y, newDir.z)));
}

/**
 * Simple. Set the y velocity to a positive value and watch the magic happend
 */
void Controller::jump()
{
    _yVelocity = JUMP_SPEED;
    _isJumping = true;
}

/**
 * Well we can't jump forever...
 */
void Controller::fall(float dt)
{
    vec3 newPos = _myCamera->getPosition();
    
    newPos.y += _yVelocity * dt;
    _yVelocity -= GRAVITY * dt;
    
    // Don't fall through the terrain
    float minHeight = PLAYER_HEIGHT;
    if (newPos.y < minHeight)
    {
        newPos.y = minHeight;
        _isJumping = false;
    }
    
    _myCamera->setPosition(newPos);
}

/**
 * Crouch to position over time
 */
void Controller::crouch(float dt)
{
    vec3 newPos = _myCamera->getPosition();
    
    newPos.y -= CROUCH_SPEED * dt;
    
    // When reaching the desired crouch height, stop the process
    float minHeight = PLAYER_CROUCH_HEIGHT;
    if (newPos.y < minHeight)
    {
        newPos.y = minHeight;
        _isCrouching = false;
        _isCrouched = true;
    }
    
    _myCamera->setPosition(newPos);
}

/**
 * Uncrouch
 */
void Controller::uncrouch(float dt)
{
    vec3 newPos = _myCamera->getPosition();
    
    newPos.y += CROUCH_SPEED * dt;
    
    float maxHeight = PLAYER_HEIGHT;
    if (newPos.y > maxHeight)
    {
        newPos.y = maxHeight;
        _isCrouching = false;
        _isCrouched = false;
    }
    
    _myCamera->setPosition(newPos);
}

