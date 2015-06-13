//
//  Camera.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Camera.h"
#include <iostream>

#define FRUSTUM_ANGLE (45.0f)
#define NEAR_PLANE (0.1f)
#define FAR_PLANE (10000.0f)

Camera::Camera(vec3 position, vec3 direction, vec3 up) :
_frustumAngle(FRUSTUM_ANGLE),
_nearPlane(NEAR_PLANE), _farPlane(FAR_PLANE),
_aspectRatio(1.0f),
_position(position), _direction(direction), _up(up),
_view(lookAt(position, position + _direction, _up)),
_projection(perspective(_frustumAngle, 1.0f, _nearPlane, _farPlane)),
_viewProjection(_projection * _view)
{
}

Camera::~Camera(){}

static Camera *mainCamera;
Camera* Camera::MainCamera()
{
    if (mainCamera == NULL) {
        std::cout << "Trying to access Camera::MainCamera() before it was set. Please set it first!" << std::endl;
    }
    return mainCamera;
}

void Camera::setMainCamera(Camera *camera)
{
    delete mainCamera;
    mainCamera = camera;
}

float Camera::getFrustumAngle()
{
    return _frustumAngle;
}

float Camera::getNearPlane()
{
    return _nearPlane;
}

float Camera::getFarPlane()
{
    return _farPlane;
}

float Camera::getAspectRatio()
{
    return _aspectRatio;
}

/**
 * Get the position
 */
vec3 Camera::getPosition()
{
    return _position;
}

/**
 * Get the looking direction
 */
vec3 Camera::getDirection()
{
    return _direction;
}

/**
 * Get up vector
 */
vec3 Camera::getUp()
{
    return _up;
}

/**
 * Set position
 */
void Camera::setPosition(vec3 position)
{
    _position = position;
    updateViewProjection();
}

/**
 * Set direction
 */
void Camera::setDirection(vec3 direction)
{
    _direction = direction;
    updateViewProjection();
}

/**
 * Set up vector
 */
void Camera::setUp(vec3 up)
{
    _up = up;
    updateViewProjection();
}

/**
 * Update the state
 */
void Camera::updateViewProjection()
{
    _view = lookAt(_position, _position + _direction, _up);
    // No need to update the projection every time
    _viewProjection = _projection * _view;
}

/**
 * Get the view projection matrix
 */
mat4 Camera::getViewProjection()
{
    return _viewProjection;
}

/**
 * Get the view projection matrix
 */
mat4 Camera::getView()
{
    return _view;
}

/**
 * Get the view projection matrix
 */
mat4 Camera::getProjection()
{
    return _projection;
}

void Camera::resize(int screenWidth, int screenHeight)
{
    _projection = perspective(_frustumAngle, (float)screenWidth/screenHeight, _nearPlane, _farPlane);
    updateViewProjection();
    _aspectRatio = (float)screenWidth / screenHeight;
}