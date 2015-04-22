//
//  World.h
//  CGP-Ex3
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__World__
#define __CGP_Ex3__World__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
using namespace glm;

#include "Ship.h"

class World {
private:
    vec3 _startPosition;
    float _leftBound, _rightBound;
    
    Ship *_ship;
    
public:
    World();
    virtual ~World();
    void update(float dt);
    void draw();
    
    // Getters
    vec3 getStartPosition();
    Ship* getShip();
};

#endif /* defined(__CGP_Ex3__World__) */
