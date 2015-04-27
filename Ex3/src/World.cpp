//
//  World.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"
#include "Ship.h"
#include "InputManager.h"

World::World() :
SceneNode(vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f)),
_startPosition(0.0f)
{
    Ship *ship = new Ship(vec3(0.0f, 2.0f, -4.0f), vec3(0.0f, 1.0f, 0.0f), vec3(4.0f, 4.0f, 8.0f));
    addChild(ship);
}

World::~World()
{
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
}

/**
 * Render all objects in the world
 */
void World::render()
{
    
//    // Pass lighting infor to ship
//    _ship->setLightPos(vec3(0.0f));
//    _ship->setLightDir(vec3(0.0f));
//    
//    // And draw
//    _ship->draw();
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition() const
{
    return _startPosition;
}