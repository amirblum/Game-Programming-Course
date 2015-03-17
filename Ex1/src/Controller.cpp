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
#define MOVE_SPEED (0.7f)
#define MOVE_BOOST (2.5);
#define CROUCH_SPEED (0.4)
#define TURN_ANGLE (10.0f)
#define LEAN_ANGLE (5.0f)
#define JUMP_SPEED (1.5f)
#define GRAVITY (0.4f)

Controller::Controller(Camera *camera, Terrain *terrain) :
_myCamera(camera),
_terrain(terrain),
_isJumping(false),
_yVelocity(0)
{
    // Set camera to corner of terrain to begin
    float x = _terrain->getLeftBound() + 1.0f;
    float z = _terrain->getFrontBound() + 1.0f;
    float y = _terrain->interpolatedHeight(x, z) + PLAYER_HEIGHT;
    std::cout << "Initial x: " << x << ", z: " << z << std::endl;
    std::cout << "Setting initial height to: " << _terrain->interpolatedHeight(x, z) << " + " << PLAYER_HEIGHT << std::endl;
    _myCamera->setPos(vec3(x, y, z));
}

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
    
    // Jumping / crouching
    
    // Cache couch press
    bool crouchPressed = input.isPressedFirstTime(KEY_CROUCH);
    
    if (_isJumping)
    {
        fall(dt);
    }
    else if (input.isPressedFirstTime(KEY_JUMP) && !_isCrouched)
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

void Controller::move(vec3 direction)
{
    if (InputManager::Instance().isModifierPressed())
    {
        direction *= MOVE_BOOST;
    }
    
    vec3 newPos = _myCamera->getPos() + direction;
    
    // Check for bounds
    newPos.x = clamp(newPos.x, _terrain->getLeftBound(), _terrain->getRightBound());
    newPos.z = clamp(newPos.z, _terrain->getFrontBound(), _terrain->getBackBound());
    
    // Set height appropriately
    if (!_isJumping && !_isCrouching)
    {
        float playerHeight = (_isCrouched) ? PLAYER_CROUCH_HEIGHT : PLAYER_HEIGHT;
        newPos.y = _terrain->interpolatedHeight(newPos.x, newPos.z) + playerHeight;
    }
    
    // Set position
    _myCamera->setPos(newPos);
}

void Controller::turn(float angle)
{
    if (InputManager::Instance().isModifierPressed())
    {
        angle *= MOVE_BOOST;
    }
    
    vec4 newDir(_myCamera->getDir(), 1.0f);
    newDir = rotate(mat4(1.0f), angle, _myCamera->getUp()) * newDir;
    _myCamera->setDir(normalize(vec3(newDir.x, newDir.y, newDir.z)));
}

void Controller::jump()
{
    _yVelocity = JUMP_SPEED;
    _isJumping = true;
}

void Controller::fall(float dt)
{
    vec3 newPos = _myCamera->getPos();
    
    newPos.y += _yVelocity * dt;
    _yVelocity -= GRAVITY * dt;
    
    float minHeight = _terrain->interpolatedHeight(newPos.x, newPos.z) + PLAYER_HEIGHT;
    if (newPos.y < minHeight)
    {
        newPos.y = minHeight;
        _isJumping = false;
    }
    
    _myCamera->setPos(newPos);
}

void Controller::crouch(float dt)
{
    vec3 newPos = _myCamera->getPos();
    
    newPos.y -= CROUCH_SPEED * dt;
    
    float minHeight = _terrain->interpolatedHeight(newPos.x, newPos.z) + PLAYER_CROUCH_HEIGHT;
    if (newPos.y < minHeight)
    {
        newPos.y = minHeight;
        _isCrouching = false;
        _isCrouched = true;
    }
    
    _myCamera->setPos(newPos);
    
}

void Controller::uncrouch(float dt)
{
    vec3 newPos = _myCamera->getPos();
    
    newPos.y += CROUCH_SPEED * dt;
    
    float maxHeight = _terrain->interpolatedHeight(newPos.x, newPos.z) + PLAYER_HEIGHT;
    if (newPos.y > maxHeight)
    {
        newPos.y = maxHeight;
        _isCrouching = false;
        _isCrouched = false;
    }
    
    _myCamera->setPos(newPos);
}

