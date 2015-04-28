//
//  Camera.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Camera.h"
#include <iostream>

Camera::Camera(vec3 position, vec3 direction, vec3 up) :
SceneNode(position, vec3(0.0f, 1.0f, 0.0f), vec3(1.0f)),
_direction(direction),
_up(up),
_view(lookAt(position, position + _direction, _up)),
_projection(perspective(45.0f, 1.0f, 0.1f, 1000.0f)),
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
    mainCamera = camera;
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

void Camera::resize(int screenWidth, int screenHeight)
{
    _projection = perspective(45.0f, (float)screenWidth/screenHeight, 0.1f, 1000.0f);
    updateViewProjection();
}