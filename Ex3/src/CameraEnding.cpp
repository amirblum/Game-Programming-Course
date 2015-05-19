//
//  CameraEnding.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraEnding.h"
#include "GameState.h"

#define CAMERA_DISTANCE (2.5f)
#define TRAVEL_TIME (1.0f)

CameraEnding::CameraEnding(Camera *camera, Ship *ship, SkyBox *skyBox) :
_camera(camera), _ship(ship), _skyBox(skyBox),
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
        _startOffset = _camera->getPosition() - _ship->getPosition();
        _endOffset = -_ship->getRight() * CAMERA_DISTANCE;
        _startingUp = _camera->getUp();
        _started = true;
    }
    
    // Get current
    vec3 cameraPosition = _camera->getPosition();
    vec3 shipPosition = _ship->getPosition();
    
    // Calculate relative start/end
    vec3 startPosition = shipPosition + _startOffset;
    vec3 endPosition = shipPosition + _endOffset;
    
    _currentTravelTime += dt;
    if (_currentTravelTime > TRAVEL_TIME) {
        _currentTravelTime = TRAVEL_TIME;
    }
    float travelPercent = _currentTravelTime / TRAVEL_TIME;
    
    cameraPosition = mix(startPosition, endPosition, travelPercent);
    
    vec3 cameraToShip = shipPosition - cameraPosition;
    if (length(cameraToShip) < CAMERA_DISTANCE) {
        cameraPosition = shipPosition - normalize(cameraToShip) * CAMERA_DISTANCE;
    }
    
    _camera->setPosition(cameraPosition);
    _camera->setDirection(normalize(_ship->getPosition() - cameraPosition));
    _camera->setUp(mix(_startingUp, cross(_ship->getForward(), _ship->getRight()), travelPercent));
    
    // Also, move the skybox
    _skyBox->setPosition(cameraPosition);
}