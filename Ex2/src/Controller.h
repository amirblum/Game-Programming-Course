//
//  Controller.h
//  CGP-Ex2
//
//  A character controller class. Interprets the input and moves the
//  camera along accordingly.
//
//  Created by Amir Blum on 3/17/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex2__Controller__
#define __CGP_Ex2__Controller__


#include "InputManager.h"

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
    
    // Jumping
    bool _isJumping;
    float _yVelocity;
    
    // Crouching
    bool _isCrouching;
    bool _isCrouched;
    
public:
    Controller(Camera *camera, World *world);
    ~Controller();
    
    void update(float dt);
    
private:
    // Private functions
    void move(vec3 direction);
    void moveToPosition(vec3 position);
    void turn(float angle);
    
    void jump();
    void fall(float dt);
    
    void crouch(float dt);
    void uncrouch(float dt);
};

#endif /* defined(__CGP_Ex2__Controller__) */
