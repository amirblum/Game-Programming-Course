//
//  Camera.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Camera.h"

Camera::Camera() {
    _dir = vec3(0.0f, 0.0f, 1.0f);
    _pos = vec3(0.0f, 1.0f, -5.0f);
    _up = vec3(0.0f, 1.0f, 0.0f);
    
    _view = lookAt(_pos, _pos + _dir, _up);
    _projection = perspective(45.0f, 1.0f, 0.1f, 100.0f);
    _viewProjection = _projection * _view;
}

mat4 Camera::GetViewProjection() {
    return _viewProjection;
}