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
#include "CameraScripts.h"
#include "GameOver.h"
#include "GameState.h"

static const std::string BACKGROUND_MUSIC = "assets/sounds/BSG_battle.wav";

World::World() :
SceneNode(),
_startPosition(0.0f), _started(false)
{
//    GLenum error;
//    error = glGetError();
//    while (error != GL_NO_ERROR) {
//        std::cout << "1. " << gluErrorString(error) << std::endl;
//        error = glGetError();
//    }
    // Camera
    Camera *camera = new Camera(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
    Camera::setMainCamera(camera);
    
    _ship = new Ship(vec3(0.0f), quat(vec3(0.0f)), vec3(1.0f, 1.0f, 1.0f), /*radius*/1.0f);
    addChild(_ship);
    
//    DummyObject *dummy = new DummyObject(vec3(-1.0f, 0.0f, 1.0f), quat(vec3(0.0f)), vec3(1.0f));
//    addChild(dummy);
    
    AsteroidParticleSystem *asteroids = new AsteroidParticleSystem(10000, 1000.0f, _ship);
    addChild(asteroids);
    
    // Skybox (draw last to save on fragments that already have info in them)
    SkyBox *skybox = new SkyBox();
    addChild(skybox);
    
    // Scripts
    addScript(new CameraScripts(camera, _ship, skybox));
    
    // Background music
    _backgroundMusicBuffer = alutCreateBufferFromFile(BACKGROUND_MUSIC.c_str());
    ALenum alutError = alutGetError();
    if (alutError != AL_NO_ERROR) {
        std::cout << "Error loading background music: " << alutGetErrorString(alutError) << std::endl;
    }
    
    alGenSources(1, &_backgroundMusicSource);
    alSourcei(_backgroundMusicSource, AL_BUFFER, _backgroundMusicBuffer);
    alSourcei(_backgroundMusicSource, AL_LOOPING, true);
    
    GameState::Instance().reset();
}

World::~World()
{
    alSourceStop(_backgroundMusicSource);
    alDeleteBuffers(1, &_backgroundMusicBuffer);
    alDeleteSources(1, &_backgroundMusicSource);
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
    GameState &state = GameState::Instance();
    
    if (state.gameStarted) {
        if (!_started) {
            alSourcePlay(_backgroundMusicSource);
            _started = true;
        }
        
        if (!state.gameOver && InputManager::Instance().isPressedFirstTime(KEY_GAME_OVER)) {
            _ship->die();
        }
        
        if (!state.gameOver && _ship->isDead()) {
            GameOver *gameOverText = new GameOver();
            addChild(gameOverText);
            state.gameOver = true;
        }
    }
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition() const
{
    return _startPosition;
}