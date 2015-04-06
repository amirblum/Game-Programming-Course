//
//  World.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"

#define CORRIDOR_WIDTH (5.0f)
#define CORRIDOR_HEIGHT (5.0f)
#define CORRIDOR_LENGTH (100.0f)

World::World() :
_corridor(CORRIDOR_WIDTH, CORRIDOR_HEIGHT, CORRIDOR_LENGTH),
_startPosition(0.0f, 0.0f, 0.0f)
{
}

void World::update(float dt)
{
    
}

void World::draw()
{
    _corridor.draw();
}

vec3 World::getStartPosition()
{
    return _startPosition;
}