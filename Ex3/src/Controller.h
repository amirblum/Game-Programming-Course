//
//  Controller.h
//  CGP-Ex3
//
//  A character controller class. Interprets the input and moves the
//  camera along accordingly.
//
//  Created by Amir Blum on 3/17/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__Controller__
#define __CGP_Ex3__Controller__


#include "Camera.h"
#include "World.h"

#include <GL/glew.h>
// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Controller {
    Camera *_myCamera;
    World *_world;
    
public:
    Controller(Camera *camera, World *world);
    ~Controller();
    
    void update(float dt);
    
private:
    // Private functions
    void move(vec3 direction);
    void strafe(vec3 direction);
    void turn(float angle);
};

#endif /* defined(__CGP_Ex3__Controller__) */
