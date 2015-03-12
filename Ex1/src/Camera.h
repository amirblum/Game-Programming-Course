//
//  Camera.h
//  CGP-Ex1
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__Camera__
#define __CGP_Ex1__Camera__

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

class Camera {

public:
    static Camera& Instance() {
        static Camera instance;
        
        return instance;
    }
    mat4 GetViewProjection();

private:
    vec3 _dir;
    vec3 _pos;
    vec3 _up;
    
    mat4 _view;
    mat4 _projection;
    mat4 _viewProjection;
    
    Camera();
    Camera(Camera const&);
    void operator=(Camera const&);
    
};

#endif /* defined(__CGP_Ex1__Camera__) */
