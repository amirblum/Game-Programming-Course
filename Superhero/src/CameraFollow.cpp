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

CameraFollow::CameraFollow(Camera *camera, Superhero *superhero, SkyBox *skybox) :
_camera(camera), _superhero(superhero),
_previousFromSuperhero(camera->getPosition() - superhero->getWorldPosition()),
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
    vec3 superheroPosition = _superhero->getPosition();
    vec3 superheroForward = _superhero->getForward();
    vec3 superheroRight = _superhero->getRight();
    
//    vec3 cameraPositionTarget;
    vec3 fromSuperheroTarget;
    vec3 cameraDirectionTarget;
    vec3 cameraUpTarget = cross(superheroForward, superheroRight);
    
    vec3 vecToSuperhero = superheroPosition - cameraPosition;
    float distanceFromSuperhero = length(vecToSuperhero);
    if (distanceFromSuperhero > 0) {
//        float cosCameraAngle = dot(superheroForward, normalize(vecToSuperhero));
//        float relativeDistanceFromSuperhero = distanceFromSuperhero * cosCameraAngle;
        if (distanceFromSuperhero < MIN_CAMERA_DISTANCE){
            fromSuperheroTarget = -superheroForward * MIN_CAMERA_DISTANCE;
            cameraDirectionTarget = superheroForward;
        } else {
            float desiredDistanceFromSuperhero = mix(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, _superhero->getSpeed() / _superhero->getMaxSpeed());
//            float distanceFromDesiredDistance = relativeDistanceFromSuperhero - desiredDistanceFromSuperhero;
//            float distanceFromTarget = distanceFromDesiredDistance / POSITION_FOLLOW_PERCENT;
//            
//            float targetOffset = distanceFromTarget - distanceFromSuperhero;
//            cameraPositionTarget = superheroPosition + superheroForward * targetOffset;
            fromSuperheroTarget = -superheroForward * desiredDistanceFromSuperhero;
            
            cameraDirectionTarget = superheroForward;
        }
    }
    
    
    // Interpolate
//    vec3 cameraPositionInterpolated = mix(cameraPosition, cameraPositionTarget, POSITION_FOLLOW_PERCENT);
    vec3 fromSuperheroInterpolated = mix(_previousFromSuperhero, fromSuperheroTarget, POSITION_FOLLOW_PERCENT);
    _previousFromSuperhero = fromSuperheroInterpolated;
    
    vec3 cameraPositionInterpolated = superheroPosition + fromSuperheroInterpolated;
    vec3 cameraDirectionInterpolated = mix(cameraDirection, cameraDirectionTarget, ROTATION_FOLLOW_PERCENT);
    vec3 cameraUpInterpolated = mix(cameraUp, cameraUpTarget, ROTATION_FOLLOW_PERCENT);
        
    _camera->setPosition(cameraPositionInterpolated);
    _camera->setDirection(cameraDirectionInterpolated);
    _camera->setUp(cameraUpInterpolated);
    
    // Also, move the skybox
    _skybox->setPosition(cameraPositionInterpolated);
}