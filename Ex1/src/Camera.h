//
//  Camera.h
//  CGP-Ex1
//
//  Camera singleton. Doesn't do much on it's own, but is easily accesible in
//  the rest of the project.
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__Camera__
#define __CGP_Ex1__Camera__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "InputManager.h"

class Camera {

public:
    // Singleton implementation
    static Camera& Instance() {
        static Camera instance;
        
        return instance;
    }
    
    // Getters
    vec3 getPos();
    vec3 getDir();
    vec3 getUp();
    mat4 getViewProjection();
    
    // Setters
    void setDir(vec3 dir);
    void setPos(vec3 pos);
    void setUp(vec3 up);

private:
    vec3 _pos;
    vec3 _dir;
    vec3 _up;
    
    mat4 _view;
    mat4 _projection;
    mat4 _viewProjection;
    
    // Singleton private initialization
    Camera();
    Camera(Camera const&);
    void operator=(Camera const&);
    
    // Update state
    void updateViewProjection();
};

#endif /* defined(__CGP_Ex1__Camera__) */
