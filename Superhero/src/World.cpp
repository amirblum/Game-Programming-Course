//
//  World.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"
#include "Controller.h"
#include "TargetFollow.h"
#include "SkyBox.h"
#include "City.h"
#include "Superhero.h"
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
    Camera *camera = new Camera();
    Camera::setMainCamera(camera);
    
    // City
    City *city = new City(4, 4);
    
    // Superhero
    _superhero = new Superhero(vec3(15.0f, 10.0f, 0.0f), quat(vec3(0.0f)), vec3(1.0f, 1.0f, 1.0f), /*radius*/1.6f);
    
    // Skybox (draw last to save on fragments that already have info in them)
    SkyBox *skybox = new SkyBox();
    
    // Add children
    {
        addChild(city);
        addChild(_superhero);
        addChild(camera);
        camera->addChild(skybox);
        // Transparent things should be rendered after skybox
    }
    
    // Scripts
    {
        addScript(new CameraScripts(camera, _superhero));
//        addScript(new TargetFollow(skybox, camera));
    //    addScript(new Controller(camera));
    }
    
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