//
//  Camera.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Camera.h"

Camera::Camera() :
_dir(0.0f, 0.0f, 1.0f),
_pos(0.0f, 1.0f, -5.0f),
_up(0.0f, 1.0f, 0.0f),
_view(lookAt(_pos, _pos + _dir, _up)),
_projection(perspective(45.0f, 1.0f, 0.1f, 1000.0f)),
_viewProjection(_projection * _view)
{
}

vec3 Camera::getDir()
{
    return _dir;
}

vec3 Camera::getPos()
{
    return _pos;
}

vec3 Camera::getUp()
{
    return _up;
}

void Camera::setDir(vec3 dir)
{
    _dir = dir;
    updateViewProjection();
}

void Camera::setPos(vec3 pos)
{
    _pos = pos;
    updateViewProjection();
}

void Camera::setUp(vec3 up)
{
    _up = up;
    updateViewProjection();
}

void Camera::updateViewProjection()
{
    _view = lookAt(_pos, _pos + _dir, _up);
    // No need to update the projection every time
    _viewProjection = _projection * _view;
}

mat4 Camera::getViewProjection()
{
    return _viewProjection;
}