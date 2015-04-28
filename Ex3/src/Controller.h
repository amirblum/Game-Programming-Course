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


#include "Ship.h"
#include "Camera.h"

#include <GL/glew.h>
// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Controller {
private:
    Ship *_ship;
    // Private functions
    void move(vec3 direction);
    void strafe(vec3 direction);
    void turn(float angle);
    
public:
    Controller(Ship *ship, vec3 startPosition);
    ~Controller();
    
    void update(float dt);
};

#endif /* defined(__CGP_Ex3__Controller__) */
