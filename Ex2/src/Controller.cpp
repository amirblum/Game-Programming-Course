//
//  Controller.cpp
//  CGP-Ex2
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
_world(world),
_lightOffset(LIGHT_OFFSET_X, LIGHT_OFFSET_Y, LIGHT_OFFSET_Z),
_currentBobAmount(0.0f),
_leaningLeft(true),
_unBobbing(false),
_jumpScare(false),
_jumpScareTime(0.0f),
_timeSinceLightFlicker(0.0f),
_timeTillLightFlicker(nextLightFlickerTime())
{
    vec3 startPosition = world->getStartPosition();
    startPosition.x -= (BOB_SPEED / BOB_MAX);
    vec3 cameraPosition = vec3(startPosition.x, startPosition.y + PLAYER_HEIGHT, startPosition.z);
    
    // Set camera to middle of corridor
    _myCamera->setPos(cameraPosition);
    
    // Set the flashlight
    _flashlight = _world->getFlashlight();
    _flashlight->setPosition(cameraPosition + _lightOffset);
}

/**
 * The update function. Every frame check inputs and move camera around accordingly.
 */
void Controller::update(float dt)
{
    InputManager &input = InputManager::Instance();
    
    // Flashlight
    _world->moveLight(input.getMousePos());
    
    if (!_jumpScare && input.isPressedFirstTime(KEY_FLICKER)) {
        _jumpScareTime = 0.0f;
        _jumpScare = true;
    } else if (_jumpScare && _jumpScareTime < JUMP_SCARE_TIME) {
        _jumpScareTime += (dt / 10.0f); // Fix to 1 sec
        if (_jumpScareTime >= JUMP_SCARE_TIME) {
            _jumpScare = true;
            _flashlight->scaryFlicker();
            _world->jumpScare();
        }
    }
    
    if (!(_jumpScare && _jumpScareTime >= JUMP_SCARE_TIME)) {
        _timeSinceLightFlicker += (dt / 10.0f); // Fix to 1 sec
        if (_timeSinceLightFlicker > _timeTillLightFlicker) {
            // Reset timer
            _timeTillLightFlicker = nextLightFlickerTime();
            _timeSinceLightFlicker = 0.0f;
            
            // Flicker
            _world->getFlashlight()->randomFlicker();
        }
    }
    
    // Moving
    if (!(_jumpScare && _jumpScareTime >= JUMP_SCARE_TIME) && input.isPressed(KEY_FORWARD))
    {
        // TODO: Accelerate slightly at first
        advance(true, dt);
    }
    else if (!(_jumpScare && _jumpScareTime >= JUMP_SCARE_TIME) && input.isPressed(KEY_BACKWARD))
    {
        advance(false, dt);
    }
    
    // Turning (a debug move :) )
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
        strafe(-cross(_myCamera->getDir(), _myCamera->getUp()) * STRAFE_SPEED * dt);
    }
    else if (input.isPressed(KEY_STRAFE_RIGHT))
    {
        strafe(cross(_myCamera->getDir(), _myCamera->getUp()) * STRAFE_SPEED * dt);
    }
    
    // Unbob
    if (_unBobbing) {
        unBob(dt);
    }
}

/**
 * Advance down the corridor
 */
void Controller::advance(bool forward, float dt)
{
    float amount = -MOVE_SPEED * dt;
    float bobAmount = BOB_SPEED * dt;

    if (!forward) amount = -amount;
    
    // Check for boost;
    if (InputManager::Instance().isModifierPressed())
    {
        amount *= MOVE_BOOST;
        bobAmount *= MOVE_BOOST;
    }
    
    _world->advanceCorridor(amount);
    
    // Bob up
    if (!_unBobbing) {
        float leanAmount = (_leaningLeft) ? bobAmount : -bobAmount;
        vec3 cameraPos = _myCamera->getPos();
        cameraPos.y += bobAmount;
        cameraPos.x += leanAmount;
        
        _myCamera->setPos(cameraPos);
        
        if (cameraPos.y - PLAYER_HEIGHT > BOB_MAX) {
            _leaningLeft = !_leaningLeft;
            _unBobbing = true;
        }
    }
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
    vec3 newLightPos = newCameraPos + _lightOffset;
    
    newCameraPos.x = clamp(newCameraPos.x, _world->getLeftBound(), _world->getRightBound());
    newLightPos.x = clamp(newLightPos.x, _world->getLeftBound(), _world->getRightBound());
    
    _myCamera->setPos(newCameraPos);
    _flashlight->setPosition(newLightPos);
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

void Controller::unBob(float dt)
{
    float bobAmount = BOB_SPEED * dt;
    
    // Check for boost;
    if (InputManager::Instance().isModifierPressed())
    {
        bobAmount *= MOVE_BOOST;
    }
    
    vec3 cameraPos = _myCamera->getPos();
    cameraPos.y -= bobAmount * 4.0f;
    if (cameraPos.y < PLAYER_HEIGHT) {
        cameraPos.y = PLAYER_HEIGHT;
        _unBobbing = false;
    }
    
    _myCamera->setPos(cameraPos);
}

float Controller::nextLightFlickerTime()
{
    float scale = ((float)rand()) / (float)RAND_MAX;
    float range = RANGE_LIGHT_FLICKER_TIME * 2 * scale - RANGE_LIGHT_FLICKER_TIME;
    return AVERAGE_LIGHT_FLICKER_TIME + range;
}

