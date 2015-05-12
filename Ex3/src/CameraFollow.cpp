//
//  CameraFollow.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraFollow.h"
#include "GameState.h"

#define MIN_CAMERA_DISTANCE (4.0f)
#define MAX_CAMERA_DISTANCE (10.0f)
#define MAX_LAG_SPEED (20.0f)
#define FOLLOW_PERCENT (0.05f)

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
    
    vec3 cameraPositionTarget;
    vec3 cameraDirectionTarget;
    vec3 cameraUpTarget = cross(shipForward, shipRight);
    
    if (!GameState::Instance().gameOver) {
        cameraDirectionTarget = shipForward;
        
        vec3 vecToShip = shipPosition - cameraPosition;
        float distanceFromShip = length(vecToShip);
        if (distanceFromShip > 0) {
            float cosCameraAngle = dot(shipForward, normalize(vecToShip));
            float relativeDistanceFromShip = distanceFromShip * cosCameraAngle;
            
            float desiredDistanceFromShip = mix(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, _ship->getSpeed() / _ship->getMaxSpeed());
            float distanceFromDesiredDistance = relativeDistanceFromShip - desiredDistanceFromShip;
            float distanceFromTarget = distanceFromDesiredDistance / FOLLOW_PERCENT;
            
            float targetOffset = distanceFromTarget - distanceFromShip;
            cameraPositionTarget = shipPosition + shipForward * targetOffset;
        }
    } else {
        // View burning ship from the side
        cameraPosition = shipPosition - shipRight * MIN_CAMERA_DISTANCE;
        cameraPositionTarget = cameraPosition;
        cameraDirectionTarget = shipRight;
    }
    
    // Interpolate
    vec3 cameraPositionInterpolated = mix(cameraPosition, cameraPositionTarget, FOLLOW_PERCENT);
    vec3 cameraDirectionInterpolated = mix(cameraDirection, cameraDirectionTarget, FOLLOW_PERCENT);
    vec3 cameraUpInterpolated = mix(cameraUp, cameraUpTarget, FOLLOW_PERCENT);
        
    _camera->setPosition(cameraPositionInterpolated);
    _camera->setDirection(cameraDirectionInterpolated);
    _camera->setUp(cameraUpInterpolated);
    
    // Also, move the skybox
    _skybox->setPosition(cameraPositionInterpolated);
}