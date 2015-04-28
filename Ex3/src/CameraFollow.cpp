//
//  CameraFollow.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraFollow.h"

CameraFollow::CameraFollow(Camera *camera, Ship *ship) :
_camera(camera), _ship(ship)
{
}

CameraFollow::~CameraFollow(){};

void CameraFollow::update(float dt)
{
    vec3 shipPosition = _ship->getPosition();
    vec3 cameraPosition = shipPosition - _ship->getForward() * 5.0f;
    _camera->setPosition(cameraPosition);
    _camera->setDirection(_ship->getForward());
}