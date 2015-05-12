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
    _camera->setPosition(_ship->getPosition() - _ship->getForward() * MIN_CAMERA_DISTANCE);
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
        cameraPositionTarget = shipPosition - shipForward * MIN_CAMERA_DISTANCE;
        cameraDirectionTarget = shipForward;
        
        // Clamp camera lag
        vec3 vecToTarget = cameraPositionTarget - cameraPosition;
        float distanceFromTarget = length(vecToTarget);
        if (distanceFromTarget > 0 && _ship->getSpeed() > MAX_LAG_SPEED) {
            float cosCameraAngle = dot(shipForward, normalize(vecToTarget));
            float relativeDistanceFromTarget = distanceFromTarget * cosCameraAngle;
            float allowedDistanceFromTarget = MAX_CAMERA_DISTANCE - MIN_CAMERA_DISTANCE;
            
            if (relativeDistanceFromTarget > allowedDistanceFromTarget) {
                float distanceFromMax = relativeDistanceFromTarget - allowedDistanceFromTarget;
                float desiredDistanceFromTarget = distanceFromMax / FOLLOW_PERCENT;
                float targetOffset = desiredDistanceFromTarget - relativeDistanceFromTarget;
                cameraPositionTarget += shipForward * targetOffset;
            }
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