//
//  CameraFollow.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraFollow.h"

#define CAMERA_DISTANCE (7.5f)
#define FOLLOW_SPEED (0.25f)

CameraFollow::CameraFollow(Camera *camera, Ship *ship, SkyBox *skybox) :
_camera(camera), _ship(ship), _skybox(skybox)
{
}

CameraFollow::~CameraFollow()
{};

void CameraFollow::update(float dt)
{
    // Get current
    vec3 cameraPosition = _camera->getPosition();
    vec3 cameraDirection = _camera->getDirection();
    vec3 cameraUp = _camera->getUp();
    
    // Get target
    vec3 shipPosition = _ship->getPosition();
    vec3 shipForward = _ship->getForward();
    vec3 shipRight = _ship->getRight();
    
    vec3 cameraPositionTarget = shipPosition - shipForward * CAMERA_DISTANCE;
    vec3 cameraDirectionTarget = shipForward;
    vec3 cameraUpTarget = cross(shipForward, shipRight);
    
    // Interpolate
    vec3 cameraPositionInterpolated = mix(cameraPosition, cameraPositionTarget, FOLLOW_SPEED * dt);
    vec3 cameraDirectionInterpolated = mix(cameraDirection, cameraDirectionTarget, FOLLOW_SPEED * dt);
    vec3 cameraUpInterpolated = mix(cameraUp, cameraUpTarget, FOLLOW_SPEED * dt);
    
    _camera->setPosition(cameraPositionInterpolated);
    _camera->setDirection(cameraDirectionInterpolated);
    _camera->setUp(cameraUpInterpolated);
    
    // Also, move the skybox
    _skybox->setPosition(cameraPositionInterpolated);
}