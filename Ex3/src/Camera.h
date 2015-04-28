//
//  Camera.h
//  CGP-Ex3
//
//  Camera singleton. Doesn't do much on it's own, but is easily accesible in
//  the rest of the project.
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__Camera__
#define __CGP_Ex3__Camera__

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

#include "SceneNode.h"
#include "InputManager.h"

class Camera : public SceneNode {
private:
    vec3 _direction;
    vec3 _up;
    
    mat4 _view;
    mat4 _projection;
    mat4 _viewProjection;
    
    // Singleton private initialization
//    Camera();
//    Camera(Camera const&);
//    void operator=(Camera const&);
    
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
    vec3 getDirection();
    vec3 getUp();
    mat4 getViewProjection();
    
    // Setters
    void setDirection(vec3 direction);
    void setUp(vec3 up);
    
    // Aspect ratio
    void resize(int screenWidth, int screenHeight);
};

#endif /* defined(__CGP_Ex3__Camera__) */
