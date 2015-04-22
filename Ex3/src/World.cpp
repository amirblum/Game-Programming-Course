//
//  World.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"
#include "InputManager.h"

World::World() :
_startPosition(0.0f)
{
    _ship = new Ship(vec3(0.0f, 2.0f, -4.0f), vec3(4.0f, 4.0f, 8.0f));
}

World::~World()
{
    delete _ship;
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
}

/**
 * Draw all objects in the world
 */
void World::draw()
{
    // Pass lighting infor to ship
    _ship->setLightPos(vec3(0.0f));
    _ship->setLightDir(vec3(0.0f));
    
    // And draw
    _ship->draw();
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition()
{
    return _startPosition;
}