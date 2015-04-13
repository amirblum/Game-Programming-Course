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


#include "Camera.h"
#include "World.h"
#include "Flashlight.h"

#include <GL/glew.h>
// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Controller {
    Camera *_myCamera;
    World *_world;
    Flashlight *_flashlight;
    vec3 _lightOffset;
    float _currentBobAmount;
    bool _leaningLeft;
    bool _unBobbing;
    float _timeSinceLightFlicker, _timeTillLightFlicker;
    
public:
    Controller(Camera *camera, World *world);
    ~Controller();
    
    void update(float dt);
    
private:
    // Private functions
    void advance(bool forward, float dt);
    void strafe(vec3 direction);
    void turn(float angle);
    void unBob(float dt);
    float nextLightFlickerTime();
    void flickerLight(float dt);
};

#endif /* defined(__CGP_Ex2__Controller__) */
