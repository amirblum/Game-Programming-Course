//
//  CameraOpening.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraOpening.h"
#include "GameState.h"

#define MIN_CAMERA_DISTANCE (4.0f)
#define MAX_CAMERA_DISTANCE (10.0f)
#define MAX_LAG_SPEED (20.0f)
#define FOLLOW_PERCENT (0.05f)

CameraOpening::CameraOpening(Camera *camera, Ship *ship, SkyBox *skyBox) :
_camera(camera), _ship(ship), _skyBox(skyBox),
_startPosition(_ship->getPosition() + _ship->getForward() * MIN_CAMERA_DISTANCE),
_endPosition(_ship->getPosition() - _ship->getForward() * MIN_CAMERA_DISTANCE),
_started(false)
{
    _camera->setPosition(_startPosition);
    _camera->setDirection(-_ship->getForward());
}

CameraOpening::~CameraOpening()
{};

void CameraOpening::update(float dt)
{
    bool actionPressed = InputManager::Instance().isPressedFirstTime(KEY_ACTION);
    
    if (!_started) {
        if (actionPressed) {
            _started = true;
        }
        return;
    }
        
    // Get current
    vec3 cameraPosition = _camera->getPosition();
    
    cameraPosition.z = mix(cameraPosition.z, _endPosition.z, 0.8f * dt);
    float travelPercent = abs(cameraPosition.z - _startPosition.z) / abs(_endPosition.z - _startPosition.z);
    cameraPosition.x = -MIN_CAMERA_DISTANCE * sin(pi<float>() * travelPercent);
    cameraPosition.y = 0.5f * MIN_CAMERA_DISTANCE * sin(pi<float>() * travelPercent);
    
    _camera->setPosition(cameraPosition);
    _camera->setDirection(normalize(_ship->getPosition() - cameraPosition));
    
    
    // Also, move the skybox
    _skyBox->setPosition(cameraPosition);
    
    float distanceFromEnd = cameraPosition.z - _endPosition.z;
    if (distanceFromEnd < 0.01f || actionPressed) {
        GameState::Instance().gameStarted = true;
    }
}