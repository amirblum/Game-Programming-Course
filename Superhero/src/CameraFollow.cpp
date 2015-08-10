//
//  CameraFollow.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraFollow.h"

#define MIN_CAMERA_DISTANCE (4.0f)
#define MAX_CAMERA_DISTANCE (10.0f)
#define MAX_LAG_SPEED (20.0f)
#define POSITION_FOLLOW_PERCENT (0.085f)
#define ROTATION_FOLLOW_PERCENT (0.07f)

CameraFollow::CameraFollow(Camera *camera, Ship *ship, SkyBox *skybox) :
_camera(camera), _ship(ship),
_previousFromShip(camera->getPosition() - ship->getWorldPosition()),
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
    
//    vec3 cameraPositionTarget;
    vec3 fromShipTarget;
    vec3 cameraDirectionTarget;
    vec3 cameraUpTarget = cross(shipForward, shipRight);
    
    vec3 vecToShip = shipPosition - cameraPosition;
    float distanceFromShip = length(vecToShip);
    if (distanceFromShip > 0) {
//        float cosCameraAngle = dot(shipForward, normalize(vecToShip));
//        float relativeDistanceFromShip = distanceFromShip * cosCameraAngle;
        if (distanceFromShip < MIN_CAMERA_DISTANCE){
            fromShipTarget = -shipForward * MIN_CAMERA_DISTANCE;
            cameraDirectionTarget = shipForward;
        } else {
            float desiredDistanceFromShip = mix(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, _ship->getSpeed() / _ship->getMaxSpeed());
//            float distanceFromDesiredDistance = relativeDistanceFromShip - desiredDistanceFromShip;
//            float distanceFromTarget = distanceFromDesiredDistance / POSITION_FOLLOW_PERCENT;
//            
//            float targetOffset = distanceFromTarget - distanceFromShip;
//            cameraPositionTarget = shipPosition + shipForward * targetOffset;
            fromShipTarget = -shipForward * desiredDistanceFromShip;
            
            cameraDirectionTarget = shipForward;
        }
    }
    
    
    // Interpolate
//    vec3 cameraPositionInterpolated = mix(cameraPosition, cameraPositionTarget, POSITION_FOLLOW_PERCENT);
    vec3 fromShipInterpolated = mix(_previousFromShip, fromShipTarget, POSITION_FOLLOW_PERCENT);
    _previousFromShip = fromShipInterpolated;
    
    vec3 cameraPositionInterpolated = shipPosition + fromShipInterpolated;
    vec3 cameraDirectionInterpolated = mix(cameraDirection, cameraDirectionTarget, ROTATION_FOLLOW_PERCENT);
    vec3 cameraUpInterpolated = mix(cameraUp, cameraUpTarget, ROTATION_FOLLOW_PERCENT);
        
    _camera->setPosition(cameraPositionInterpolated);
    _camera->setDirection(cameraDirectionInterpolated);
    _camera->setUp(cameraUpInterpolated);
    
    // Also, move the skybox
    _skybox->setPosition(cameraPositionInterpolated);
}