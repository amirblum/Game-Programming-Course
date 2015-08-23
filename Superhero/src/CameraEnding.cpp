//
//  CameraEnding.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraEnding.h"
#include "GameState.h"

#define CAMERA_DISTANCE (2.5f)
#define TRAVEL_TIME (1.0f)

CameraEnding::CameraEnding(Camera *camera, Superhero *superhero) :
_camera(camera), _superhero(superhero),
_startOffset(0.0f),
_endOffset(0.0f),
_startingUp(0.0f),
_currentTravelTime(0.0f),
_started(false)
{
}

CameraEnding::~CameraEnding()
{}

void CameraEnding::update(float dt)
{
    if (!_started) {
        _startOffset = _camera->getPosition() - _superhero->getPosition();
        _endOffset = vec3(1.0f, 0.0f, 0.0f) * CAMERA_DISTANCE;
        _startingUp = _camera->getUp();
        _started = true;
    }
    
    // Get current
    vec3 cameraPosition = _camera->getPosition();
    vec3 superheroPosition = _superhero->getPosition();
    
    // Calculate relative start/end
    vec3 startPosition = superheroPosition + _startOffset;
    vec3 endPosition = superheroPosition + _endOffset;
    
    _currentTravelTime += dt;
    if (_currentTravelTime > TRAVEL_TIME) {
        _currentTravelTime = TRAVEL_TIME;
    }
    float travelPercent = _currentTravelTime / TRAVEL_TIME;
    
    cameraPosition = mix(startPosition, endPosition, travelPercent);
    
    vec3 cameraToSuperhero = superheroPosition - cameraPosition;
    if (length(cameraToSuperhero) < CAMERA_DISTANCE) {
        cameraPosition = superheroPosition - normalize(cameraToSuperhero) * CAMERA_DISTANCE;
    }
    
    _camera->setPosition(cameraPosition);
    _camera->setDirection(normalize(_superhero->getPosition() - cameraPosition));
}
