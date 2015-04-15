//
//  Flashlight.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 4/13/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Flashlight.h"

#include <iostream>

// Constructor
Flashlight::Flashlight(float cutoff, float intensity) :
_cutoffAngle(cutoff),
_currentIntensity(intensity),
_maxIntensity(intensity),
_position(0.0f),
_direction(0.0f, 0.0f, 1.0f),
_state(FLICKER_NONE)
{
}

// Getters
float Flashlight::getCutoff()
{
    return _cutoffAngle;
}

float Flashlight::getIntensity()
{
    return _currentIntensity;
}

vec3 Flashlight::getPosition()
{
    return _position;
}

vec3 Flashlight::getDirection()
{
    return _direction;
}

// Setters
void Flashlight::setCutoff(float newCutoff)
{
    if (newCutoff > 0.0f) {
        _cutoffAngle = newCutoff;
    }
}

void Flashlight::setIntensity(float newIntensity)
{
    _currentIntensity = newIntensity;
    _currentIntensity = clamp(_currentIntensity, 0.0f, _maxIntensity);
}

void Flashlight::setPosition(vec3 newPosition)
{
    _position = newPosition;
}

void Flashlight::setDirection(vec3 newDirection)
{
    _direction = newDirection;
}

// Utility methods
void Flashlight::dim(float targetIntensity, float speed)
{
    Flicker flicker;
    flicker.waitTime = 0.0f;
    flicker.targetIntensity = targetIntensity;
    flicker.speed = speed;
    flicker.recover = false;
    _flickerQueue.push(flicker);
}

void Flashlight::randomFlicker()
{
    int flickerNum = 1 + rand() % 4;
    for (int i = 0; i < flickerNum; ++i) {
        Flicker flicker;
        // Between 0.5-1.0
        float randomIntensity = (((float)rand() / RAND_MAX) + 1.0f) / 2.0f;
        // Between 1.0-2.0
        float randomSpeed = (float)rand() / RAND_MAX + 1.0f;
        
        flicker.waitTime = 0.0f;
        flicker.targetIntensity = randomIntensity * _maxIntensity;
        flicker.speed = randomSpeed;
        // On last flicker, chance of not recovering
        if (i == flickerNum - 1) {
            // 0-1 recover
            bool recover = rand() % 3 == 0;
            flicker.recover = recover;
        } else {
            flicker.recover = true;
        }
        
        _flickerQueue.push(flicker);
    }
}

void Flashlight::scaryFlicker()
{
    // Push three flickers:
    Flicker flicker1, flicker2, flicker3, recover;

    flicker1.waitTime = 0.0f;
    flicker1.targetIntensity = 0.5f * _maxIntensity;
    flicker1.speed = 1.0f;
    flicker1.recover = true;
    
    flicker2.waitTime = 0.0f;
    flicker2.targetIntensity = 0.75f * _maxIntensity;
    flicker2.speed = 2.0f;
    flicker1.recover = true;
    
    flicker3.waitTime = 1.0f;
    flicker3.targetIntensity = 0.0f;
    flicker3.speed = 0.5f;
    flicker3.recover = false;
    
    recover.waitTime = 7.0f;
    recover.targetIntensity = 0.0f;
    recover.speed = -1.0f;
    recover.recover = true;
    
    _flickerQueue.push(flicker1);
    _flickerQueue.push(flicker2);
    _flickerQueue.push(flicker3);
    _flickerQueue.push(recover);
}

// Update
void Flashlight::update(float dt)
{
    switch (_state) {
        case FLICKER_NONE:
            // Test for next flicker
            if (!_flickerQueue.empty()) {

                _state = FLICKER_WAITING;
            }
            break;
            
        case FLICKER_WAITING:
            doFlickerWait(dt);
            break;
            
        case FLICKER_DIMMING:
            doFlickerDim(dt);
            break;
    }
}

void Flashlight::doFlickerWait(float dt)
{
    Flicker flicker = _flickerQueue.front();
    
    _elapsedTime += dt;
    if (_elapsedTime > flicker.waitTime) {
        _elapsedTime = 0.0f;
        _currentIntensity = _maxIntensity;
        _state = FLICKER_DIMMING;
    }
}

void Flashlight::doFlickerDim(float dt)
{
    Flicker flicker = _flickerQueue.front();

    _currentIntensity -= flicker.speed * dt;
    if (_currentIntensity <= flicker.targetIntensity || flicker.speed < 0) {
        if (flicker.recover) {
            _currentIntensity = _maxIntensity;
        }
        
        _flickerQueue.pop();
        _state = FLICKER_NONE;
    }
}