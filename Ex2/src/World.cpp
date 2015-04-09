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
#define CORRIDOR_LENGTH (60.0f)

#define CORRIDOR_BOUND_BUFFER (1.7)

#define ADVANCE_STEP (0.1f)

World::World() :
_startPosition(0.0f),
_leftBound(-(CORRIDOR_WIDTH - CORRIDOR_BOUND_BUFFER) / 2), _rightBound((CORRIDOR_WIDTH - CORRIDOR_BOUND_BUFFER) / 2),
_corridor(vec3(0.0f, CORRIDOR_HEIGHT/2, -CORRIDOR_LENGTH/2), vec3(CORRIDOR_WIDTH, CORRIDOR_HEIGHT, CORRIDOR_LENGTH)),
_lightPos(0.0f),
_lightDir(0.0f, 0.0f, -1.0f)
{
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
    if (InputManager::Instance().isPressedFirstTime(KEY_BUMP_TOGGLE)) {
        _corridor.toggleBumpMapping();
    }
}

/**
 * Draw all objects in the world
 */
void World::draw()
{
    _corridor.draw();
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

/**
 * Advance 'amount' down the corridor
 */
void World::advanceCorridor(float amount)
{
    _corridor.increaseOffset(ADVANCE_STEP * amount);
}

/**
 * Set the position of the light
 */
void World::setLightPos(vec3 lightPos)
{
    _lightPos = lightPos;
    _corridor.setLightPos(lightPos);
}

/**
 * Sets the light cutoff angle
 */
void World::setLightCutoff(float lightCutoff)
{
    _corridor.setLightCutoff(lightCutoff);
}


/**
 * Update the light's direction
 */
void World::moveLight(vec2 mousePos)
{
    vec4 mouseInWorld = _corridor.getWorldMat() * vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
    vec3 pointAt(mouseInWorld.x, mouseInWorld.y, -_corridor.getLength() / 4);
    _lightDir = pointAt - _lightPos;
    _corridor.setLightDir(_lightDir);
}