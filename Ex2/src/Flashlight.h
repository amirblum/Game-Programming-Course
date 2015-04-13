//
//  Flashlight.h
//  CGP-Ex2
//
//  Created by Amir Blum on 4/13/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex2__Flashlight__
#define __CGP_Ex2__Flashlight__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <stdio.h>
#include <queue>

struct Flicker {
    float waitTime;
    float speed;
    float targetIntensity;
    bool recover;
};

class Flashlight {
private:
    // Info
    float _cutoffAngle;
    float _currentIntensity, _maxIntensity;
    vec3 _position, _direction;
    
    // Flickering
    enum flickerState {
        FLICKER_NONE,
        FLICKER_WAITING,
        FLICKER_DIMMING
    };
    flickerState _state;
    std::queue<Flicker> _flickerQueue;
    float _elapsedTime;
    void doFlickerWait(float dt);
    void doFlickerDim(float dt);
public:
    // Constructor
    Flashlight(float cutoff, float intensity);
    
    // Getters
    float getCutoff();
    float getIntensity();
    vec3 getPosition();
    vec3 getDirection();
    
    // Setters
    void setCutoff(float newCutoff);
    void setIntensity(float newIntensity);
    void setPosition(vec3 newPosition);
    void setDirection(vec3 newDirection);
    
    // Utility methods
    void dim(float targetIntensity, float speed);
    void scaryFlicker(float nextFlickerTime);
    
    // Update
    void update(float dt);
};

#endif /* defined(__CGP_Ex2__Flashlight__) */
