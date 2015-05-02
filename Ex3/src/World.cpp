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
#include "DummyObject.h"
#include "AsteroidParticleSystem.h"
#include "InputManager.h"

World::World() :
SceneNode(),
_startPosition(0.0f)
{
//    GLenum error;
//    error = glGetError();
//    while (error != GL_NO_ERROR) {
//        std::cout << "1. " << gluErrorString(error) << std::endl;
//        error = glGetError();
//    }

    Ship *ship = new Ship(vec3(0.0f, 0.0f, 0.0f), quat(vec3(0.0f)), vec3(1.0f, 1.5f, 4.0f));
    addChild(ship);
    
    DummyObject *dummy = new DummyObject(vec3(-1.0f, 0.0f, 1.0f), quat(vec3(0.0f)), vec3(1.0f));
    addChild(dummy);
    
    AsteroidParticleSystem *asteroids = new AsteroidParticleSystem(10, 2.0f, 10.0f, ship);
    addChild(asteroids);
        
    // Camera
    _camera = new Camera(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
    Camera::setMainCamera(_camera);
    
    // Scripts
    _cameraFollow = new CameraFollow(_camera, ship);
}

World::~World()
{
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
    _cameraFollow->update(dt);
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition() const
{
    return _startPosition;
}