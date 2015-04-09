//
//  World.h
//  CGP-Ex2
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex2__World__
#define __CGP_Ex2__World__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
using namespace glm;

#include "Corridor.h"

class World {
private:
    vec3 _startPosition;
    float _leftBound, _rightBound;
    
    Corridor _corridor;
    vec3 _lightPos;
    vec3 _lightDir;
    
public:
    World();
    void update(float dt);
    void draw();
    
    // Getters
    vec3 getStartPosition();
    float getLeftBound();
    float getRightBound();
    
    // State changers
    void advanceCorridor(float amount);
    void setLightPos(vec3 lightPos);
    void setLightCutoff(float lightCutoff);
    void moveLight(vec2 mousePos);
};

#endif /* defined(__CGP_Ex2__World__) */
