//
//  Camera.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Camera.h"

Camera::Camera() :
_dir(0.0f, 0.0f, -1.0f),
_pos(0.0f, 1.0f, 5.0f),
_up(0.0f, 1.0f, 0.0f),
_view(lookAt(_pos, _pos + _dir, _up)),
_projection(perspective(45.0f, 1.0f, 0.1f, 1000.0f)),
_viewProjection(_projection * _view)
{
}

/**
 * Get position
 */
vec3 Camera::getPos()
{
    return _pos;
}

/**
 * Get the looking direction
 */
vec3 Camera::getDir()
{
    return _dir;
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
void Camera::setDir(vec3 dir)
{
    _dir = dir;
    updateViewProjection();
}

/**
 * Set position
 */
void Camera::setPos(vec3 pos)
{
    _pos = pos;
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
    _view = lookAt(_pos, _pos + _dir, _up);
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