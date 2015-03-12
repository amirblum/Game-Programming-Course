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
_projection(perspective(45.0f, 1.0f, 0.1f, 100.0f)),
_viewProjection(_projection * _view)
{
    InputManager::Instance().registerReceiver(this);
}

void Camera::updateViewProjection() {
    _view = lookAt(_pos, _pos + _dir, _up);
    // No need to update the projection every time
    _viewProjection = _projection * _view;
}

mat4 Camera::getViewProjection() {
    return _viewProjection;
}

void Camera::receiveInput(unsigned char key, int x, int y) {
    vec4 newDir;
    switch (key) {
        case KEY_FORWARD:
            _pos += _dir;
            break;
        case KEY_BACKWARD:
            _pos -= _dir;
            break;
        case KEY_TURN_LEFT:
            newDir = vec4(_dir, 1.0f);
            newDir = rotate(mat4(1.0f), 10.0f, _up) * newDir;
            _dir = normalize(vec3(newDir.x, newDir.y, newDir.z));
            break;
        case KEY_TURN_RIGHT:
            newDir = vec4(_dir, 1.0f);
            newDir = rotate(mat4(1.0f), -10.0f, _up) * newDir;
            _dir = normalize(vec3(newDir.x, newDir.y, newDir.z));
            break;
        default:
            break;
    }
    
    updateViewProjection();
}