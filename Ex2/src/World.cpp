//
//  World.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"
#include "InputManager.h"

#define CORRIDOR_WIDTH (5.0f)
#define CORRIDOR_HEIGHT (5.0f)
#define CORRIDOR_LENGTH (120.0f)

#define CORRIDOR_BOUND_BUFFER (1.7f)

#define FLASHLIGHT_CUTOFF_ANGLE (10.0f)
#define FLASHLIGHT_INTENSITY (2.5f)
#define MOUSE_DEPTH (10.0f)

#define ADVANCE_STEP (0.1f)

World::World() :
_startPosition(0.0f),
_leftBound(-(CORRIDOR_WIDTH - CORRIDOR_BOUND_BUFFER) / 2), _rightBound((CORRIDOR_WIDTH - CORRIDOR_BOUND_BUFFER) / 2)
{
    _corridor = new Corridor(vec3(0.0f, CORRIDOR_HEIGHT/2, -CORRIDOR_LENGTH/2),
                             vec3(CORRIDOR_WIDTH, CORRIDOR_HEIGHT, CORRIDOR_LENGTH));
    _flashLight = new Flashlight(cos(radians(FLASHLIGHT_CUTOFF_ANGLE)), FLASHLIGHT_INTENSITY);
}

World::~World()
{
    delete _corridor;
    delete _flashLight;
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
    if (InputManager::Instance().isPressedFirstTime(KEY_BUMP_TOGGLE)) {
        _corridor->toggleBumpMapping();
    }
    
    // Update flashlight
    _flashLight->update(dt);
}

/**
 * Draw all shiny objects
 */
void World::drawGlow()
{
    // Pass lighting infor to corridor
    _corridor->setLightPos(_flashLight->getPosition());
    _corridor->setLightDir(_flashLight->getDirection());
    _corridor->setLightIntensity(_flashLight->getIntensity());
    _corridor->setLightCutoff(_flashLight->getCutoff());
    
    // And draw
    _corridor->drawGlow();
}

/**
 * Draw all objects in the world
 */
void World::draw()
{
    // Pass lighting infor to corridor
    _corridor->setLightPos(_flashLight->getPosition());
    _corridor->setLightDir(_flashLight->getDirection());
    _corridor->setLightIntensity(_flashLight->getIntensity());
    _corridor->setLightCutoff(_flashLight->getCutoff());
    
    // And draw
    _corridor->draw();
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition()
{
    return _startPosition;
}

/**
 * Gets the left bound
 */
float World::getLeftBound()
{
    return _leftBound;
}

/**
 * Gets the right bound
 */
float World::getRightBound()
{
    return _rightBound;
}

Corridor* World::getCorridor()
{
    return _corridor;
}

Flashlight* World::getFlashlight()
{
    return _flashLight;
}

/**
 * Advance 'amount' down the corridor
 */
void World::advanceCorridor(float amount)
{
    _corridor->increaseOffset(ADVANCE_STEP * amount);
}

/**
 * Update the light's direction
 */
void World::moveLight(vec2 mousePos)
{
    vec4 mouseInWorld = _corridor->getWorldMat() * vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
    vec3 pointAt(mouseInWorld.x, mouseInWorld.y, -MOUSE_DEPTH);
    _flashLight->setDirection(pointAt - _flashLight->getPosition());
}