//
//  CameraFollow.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraFollow.h"

#define CAMERA_DISTANCE (7.5f)

CameraFollow::CameraFollow(Camera *camera, Ship *ship) :
_camera(camera), _ship(ship)
{
}

CameraFollow::~CameraFollow(){};

void CameraFollow::update(float dt)
{
    vec3 shipPosition = _ship->getPosition();
    vec3 cameraPosition = shipPosition - _ship->getForward() * CAMERA_DISTANCE;
    vec3 shipUp = cross(_ship->getForward(), _ship->getRight());
    
    _camera->setPosition(cameraPosition);
    _camera->setDirection(_ship->getForward());
    _camera->setUp(shipUp);
}