//
//  Controller.h
//  CGP-Ex1
//
//  Created by Amir Blum on 3/17/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__Controller__
#define __CGP_Ex1__Controller__


#include "InputManager.h"

#include "Camera.h"
#include "Terrain.h"

#include <GL/glew.h>
// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Controller {
    Camera *_myCamera;
    Terrain *_terrain;
    
    // Jumping
    bool _isJumping;
    float _yVelocity;
    
    // Crouching
    bool _isCrouching;
    bool _isCrouched;
    
public:
    Controller(Camera *camera, Terrain *terrain);
    ~Controller();
    
    void update(float dt);
    
private:
    void move(vec3 direction);
    void turn(float angle);
    
    void jump();
    void fall(float dt);
    
    void crouch(float dt);
    void uncrouch(float dt);
};

#endif /* defined(__CGP_Ex1__Controller__) */
