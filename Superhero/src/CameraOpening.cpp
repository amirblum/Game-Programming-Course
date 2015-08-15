//
//  CameraOpening.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraOpening.h"
#include "GameState.h"

#define CAMERA_DISTANCE (2.5f)

CameraOpening::CameraOpening(Camera *camera, Superhero *superhero, SkyBox *skyBox) :
_camera(camera), _superhero(superhero), _skyBox(skyBox),
_startPosition(_superhero->getPosition() + _superhero->getForward() * CAMERA_DISTANCE),
_endPosition(_superhero->getPosition() - _superhero->getForward() * CAMERA_DISTANCE),
_started(false)
{
    _camera->setPosition(_startPosition);
    _camera->setDirection(-_superhero->getForward());
}

CameraOpening::~CameraOpening()
{}

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
    cameraPosition.x = -CAMERA_DISTANCE * sin(pi<float>() * travelPercent);
    cameraPosition.y = 0.5f * CAMERA_DISTANCE * sin(pi<float>() * travelPercent);
    
    _camera->setPosition(cameraPosition);
    _camera->setDirection(normalize(_superhero->getPosition() - cameraPosition));
    
    
    // Also, move the skybox
    _skyBox->setPosition(cameraPosition);
    
    float distanceFromEnd = cameraPosition.z - _endPosition.z;
    if (distanceFromEnd < 0.01f || actionPressed) {
        GameState::Instance().winState = STARTED;
    }
}