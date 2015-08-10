//
//  CameraFollow.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraFollow.h"

#define MIN_CAMERA_DISTANCE (4.0f)
#define MAX_CAMERA_DISTANCE (10.0f)
#define MAX_LAG_SPEED (20.0f)
//#define POSITION_FOLLOW_PERCENT (0.085f)
#define POSITION_FOLLOW_PERCENT (1.0f)
#define ROTATION_FOLLOW_PERCENT (0.07f)

CameraFollow::CameraFollow(Camera *camera, Ship *ship, SkyBox *skybox) :
_camera(camera), _ship(ship),
_wasResting(true), _previousRight(1.0f, 0.0f, 0.0f),
_skybox(skybox)
{
}

CameraFollow::~CameraFollow()
{}

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
    vec3 shipMovementDirection = _ship->getMovementDirection();
    
//    vec3 cameraPositionTarget = shipPosition - shipForward * MIN_CAMERA_DISTANCE;
//    vec3 cameraDirectionTarget = shipForward;
//    vec3 cameraUpTarget = cross(shipForward, shipRight);
    
    vec3 cameraPositionTarget;
    vec3 cameraDirectionTarget;
    vec3 cameraUpTarget;
    
    bool isResting = length(shipMovementDirection) < 0.1f;
    if (_wasResting && !isResting) {
        _previousRight = shipRight;
    }
    
    vec3 vecToShip = shipPosition - cameraPosition;
    float distanceFromShip = length(vecToShip);
    if (distanceFromShip > 0) {
        float cosCameraAngle;
        if (isResting) {
            cosCameraAngle = dot(shipForward, normalize(vecToShip));
        } else {
            cosCameraAngle = dot(shipMovementDirection, normalize(vecToShip));
        }
        
        float relativeDistanceFromShip = distanceFromShip * cosCameraAngle;
        if (relativeDistanceFromShip < MIN_CAMERA_DISTANCE) {
            cameraPositionTarget = shipPosition - shipForward * MIN_CAMERA_DISTANCE;
            cameraDirectionTarget = shipForward;
        } else {
            float desiredDistanceFromShip = mix(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, _ship->getSpeed() / _ship->getMaxSpeed());
            float distanceFromDesiredDistance = relativeDistanceFromShip - desiredDistanceFromShip;
            float distanceFromTarget = distanceFromDesiredDistance / POSITION_FOLLOW_PERCENT;
            
            float targetOffset = distanceFromTarget - distanceFromShip;
            if (isResting) {
                cameraPositionTarget = shipPosition + shipForward * targetOffset;
                cameraDirectionTarget = shipForward;
                cameraUpTarget = cross(shipForward, shipRight);
                _wasResting = true;
            } else {
                cameraPositionTarget = shipPosition + shipMovementDirection * targetOffset;
                cameraDirectionTarget = shipMovementDirection;
                cameraUpTarget = cross(shipMovementDirection, _previousRight);
            }

        }
    }
    
    _wasResting = isResting;
    
    
    // Interpolate
    vec3 cameraPositionInterpolated = mix(cameraPosition, cameraPositionTarget, POSITION_FOLLOW_PERCENT);
    vec3 cameraDirectionInterpolated = mix(cameraDirection, cameraDirectionTarget, ROTATION_FOLLOW_PERCENT);
    vec3 cameraUpInterpolated = mix(cameraUp, cameraUpTarget, ROTATION_FOLLOW_PERCENT);
        
    _camera->setPosition(cameraPositionInterpolated);
    _camera->setDirection(cameraDirectionInterpolated);
    _camera->setUp(cameraUpInterpolated);
    
    // Also, move the skybox
    _skybox->setPosition(cameraPositionInterpolated);
}