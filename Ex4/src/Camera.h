//
//  Camera.h
//  CGP-Ex4
//
//  Camera class. Doesn't do much on it's own, but is easily accesible in
//  the rest of the project via ability to set "MainCamera" singleton-ish.
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__Camera__
#define __CGP_Ex4__Camera__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

// GLM headers
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "InputManager.h"

class Camera {
private:
    float _frustumAngle;
    float _nearPlane, _farPlane;
    float _aspectRatio;
    
    vec3 _position;
    vec3 _direction;
    vec3 _up;
    
    mat4 _view;
    mat4 _projection;
    mat4 _viewProjection;
    
    // Update state
    void updateViewProjection();
    
public:
    // Main camera implementation
    static Camera* MainCamera();
    static void setMainCamera(Camera *camera);
    
    // Constructor
    Camera(vec3 position, vec3 direction, vec3 up);
    virtual ~Camera();
    
    // Getters
    float getFrustumAngle();
    float getNearPlane();
    float getFarPlane();
    float getAspectRatio();
    vec3 getPosition();
    vec3 getDirection();
    vec3 getUp();
    mat4 getView();
    mat4 getProjection();
    mat4 getViewProjection();
    
    // Setters
    void setPosition(vec3 position);
    void setDirection(vec3 direction);
    void setUp(vec3 up);
    
    // Aspect ratio
    void resize(int screenWidth, int screenHeight);
};

#endif /* defined(__CGP_Ex4__Camera__) */
