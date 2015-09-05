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
#define POSITION_FOLLOW_PERCENT (0.085f)
#define ROTATION_FOLLOW_PERCENT (0.07f)

CameraFollow::CameraFollow(Camera *camera, Superhero *superhero) :
_camera(camera), _superhero(superhero),
_previousFromSuperhero(camera->getPosition() - superhero->getWorldPosition())
{
}

CameraFollow::~CameraFollow()
{}

void CameraFollow::update(float dt)
{
    // Get current
    vec3 cameraPosition = _camera->getPosition();
    vec3 cameraDirection = _camera->getDirection();
    
    // Get target
    vec3 superheroPosition = _superhero->getPosition();
    vec3 superheroForward = _superhero->getForward();
    
    // Calculate desired distance
    vec3 vecToSuperhero = superheroPosition - cameraPosition;
    float distanceFromSuperhero = length(vecToSuperhero);
    float desiredDistanceFromSuperhero = distanceFromSuperhero;
    if (distanceFromSuperhero > 0) {
        if (distanceFromSuperhero < MIN_CAMERA_DISTANCE){
            desiredDistanceFromSuperhero = MIN_CAMERA_DISTANCE;
        } else {
            desiredDistanceFromSuperhero = mix(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, _superhero->getSpeed() / _superhero->getMaxSpeed());
        }
    }
    
    // Correct for frustum (dolly zoom)
    float currentFrustum = _camera->getFrustumAngle();
    float defaultFrustum = _camera->getDefaultFrustumAngle();
//    if (currentFrustum != defaultFrustum) {
    if(GameState::Instance().boostState == ZOOMING) {
        float desiredHalfWidth = desiredDistanceFromSuperhero * tan(defaultFrustum / 2.0f);
        desiredDistanceFromSuperhero = desiredHalfWidth / tan(currentFrustum / 2.0f);
    }
    
    // Interpolate
    vec3 fromSuperheroTarget = -superheroForward * desiredDistanceFromSuperhero;
    vec3 fromSuperheroInterpolated = mix(_previousFromSuperhero, fromSuperheroTarget, POSITION_FOLLOW_PERCENT);
    _previousFromSuperhero = fromSuperheroInterpolated;
    
    vec3 cameraPositionInterpolated = superheroPosition + fromSuperheroInterpolated;
    vec3 cameraDirectionInterpolated = mix(cameraDirection, superheroForward, ROTATION_FOLLOW_PERCENT);
    
    // Set
    _camera->setPosition(cameraPositionInterpolated);
    _camera->setDirection(cameraDirectionInterpolated);
}
