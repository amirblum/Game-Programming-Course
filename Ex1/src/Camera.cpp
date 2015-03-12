//
//  Camera.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Camera.h"

Camera::Camera() {
    dir = vec3(0.0f, 0.0f, 1.0f);
    pos = vec3(0.0f, 1.0f, -5.0f);
    up = vec3(0.0f, 1.0f, 0.0f);
    
    view = lookAt(pos, pos+dir, up);
    projection = perspective(45.0f, 1.0f, 0.1f, 100.0f);
    viewProjection = projection * view;
}

mat4 Camera::GetViewProjection() {
    return viewProjection;
}