//
//  World.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"
#include "Ship.h"
//#include "DummyObject.h"
#include "Beacon.h"
#include "AsteroidParticleSystem.h"
#include "BlackHole.h"
#include "InputManager.h"
#include "CameraScripts.h"
#include "Overlay.h"
#include "GameState.h"

static const std::string BACKGROUND_MUSIC = "assets/sounds/BSG_battle.wav";
static const std::string YOU_WIN_TEXTURE = "assets/youwin.png";
static const std::string GAME_OVER_TEXTURE = "assets/gameover.png";

static const vec3 BEACON_POS = vec3(-50.0f, 15.0f, 3500.0f);

World::World() :
SceneNode(),
_startPosition(0.0f), _gameStartedCheck(false), _gameOverCheck(false), _gameWonCheck(false)
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
    
//    DummyObject *dummy = new DummyObject(vec3(-1.0f, 0.0f, 1.0f), quat(vec3(0.0f)), vec3(1.0f));
//    addChild(dummy);
    
    // Skybox (draw last to save on fragments that already have info in them)
    SkyBox *skybox = new SkyBox();
    
    /**      -      Transparent things go below skybox        -      */
    
//    // Ship
//    _ship = new Ship(vec3(0.0f), quat(vec3(0.0f)), vec3(1.0f, 1.0f, 1.0f), /*radius*/1.6f);
    
    // Add children
    {
        addChild(skybox);
//        addChild(_ship);
    }
    
    // Scripts
//    addScript(new CameraScripts(camera, _ship, skybox));
    
    // Background music
    _backgroundMusic = SoundManager::Instance().loadSound(BACKGROUND_MUSIC, true);
    
    GameState::Instance().reset();
}

World::~World()
{
    SoundManager::Instance().releaseSound(_backgroundMusic);
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
    GameState &state = GameState::Instance();
    
    switch (state.winState) {
        case STARTED:
        {
            if (!_gameStartedCheck) {
                _gameStartedCheck = true;
                SoundManager::Instance().playSound(_backgroundMusic);
            }
            break;
        }
        case LOST:
        {
            if (!_gameOverCheck) {
                _gameOverCheck = true;
                Overlay *gameOverText = new Overlay(GAME_OVER_TEXTURE);
                addChild(gameOverText);
            }
            break;
        }
        case WON:
        {
            if (!_gameWonCheck) {
                _gameWonCheck = true;                
                Overlay *youWinText = new Overlay(YOU_WIN_TEXTURE);
                addChild(youWinText);
            }
            break;
        }
        default:
            break;
    }
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition() const
{
    return _startPosition;
}