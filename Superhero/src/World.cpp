//
//  World.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <iostream>

#include "World.h"
#include "TargetFollow.h"
#include "SkyBox.h"
#include "City.h"
#include "Superhero.h"
#include "InputManager.h"
#include "SuperheroPosition.h"
#include "CameraScripts.h"
#include "Overlay.h"
#include "GameState.h"

static const std::string BACKGROUND_MUSIC = "assets/sounds/hero-music.wav";
static const std::string YOU_WIN_TEXTURE = "assets/youwin.png";
static const std::string GAME_OVER_TEXTURE = "assets/gameover.png";

static const vec3 BEACON_POS = vec3(-50.0f, 15.0f, 3500.0f);

#define DOLLY_FRUSTUM_INCREASE (pi<float>() / 6.0f)
#define DOLLY_FRUSTUM_RETURN_SPEED (10.0f)
#define BOOST_MIN_ANGLE (pi<float>() / 2.8f)

World::World() :
SceneNode(),
_startPosition(0.0f), _gameStartedCheck(false), _gameOverCheck(false), _gameWonCheck(false),
_boostButtonPressed(false)
{
//    GLenum error;
//    error = glGetError();
//    while (error != GL_NO_ERROR) {
//        std::cout << "1. " << gluErrorString(error) << std::endl;
//        error = glGetError();
//    }
    
    // Camera
    _camera = new Camera();
    Camera::setMainCamera(_camera);
    
    // City
    _city = new City(5, 5);
    
    // Superhero
    _superhero = new Superhero(vec3(25.0f, 10.0f, -25.0f), quat(vec3(0.0f)), vec3(1.0f, 1.0f, 1.0f), /*radius*/1.2f);
    
    // Skybox (draw last to save on fragments that already have info in them)
    SkyBox *skybox = new SkyBox();
    
    // Add children
    {
        addChild(_city);
        addChild(_superhero);
        addChild(_camera);
        _camera->addChild(skybox);
        // Transparent things should be rendered after skybox
    }
    
    // Scripts
    {
        addScript(new SuperheroPosition(_city, _superhero));
        addScript(new CameraScripts(_camera, _superhero));
//        addScript(new TargetFollow(skybox, camera));
    //    addScript(new Controller(camera));
    }
    _city->repositionBuildings(_superhero->getPosition(), _superhero->getForward());
    
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
            } else {
                setZoom(dt);
                setCity();
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

void World::setZoom(float dt)
{
    GameState &state = GameState::Instance();
    float newFrustumAngle = _camera->getFrustumAngle();
    float maxFrustum = pi<float>() / 2.0f;
    bool wasBoostButtonPressed = _boostButtonPressed;
    _boostButtonPressed = InputManager::Instance().isPressed(KEY_ACTION);
    bool legalZoom = (!wasBoostButtonPressed && _boostButtonPressed) || (state.boostState == ZOOMING && _boostButtonPressed);
    if (legalZoom && state.boostState != BOOSTING) {
        if (state.boostState == NONE) {
            state.boostState = ZOOMING;
            _superhero->slowDown();
        }
        newFrustumAngle = min(_camera->getFrustumAngle() + DOLLY_FRUSTUM_INCREASE * dt, maxFrustum);
    }
    if (state.boostState == ZOOMING) {
        if (!_boostButtonPressed || newFrustumAngle >= (maxFrustum - epsilon<float>())) {
            if (newFrustumAngle > BOOST_MIN_ANGLE) {
                state.boostState = BOOSTING;
                _superhero->boost(newFrustumAngle / maxFrustum);
            } else {
                state.boostState = NONE;
                _superhero->unslow();
            }
        }
    } else {
        newFrustumAngle = mix(_camera->getFrustumAngle(), _camera->getDefaultFrustumAngle(), DOLLY_FRUSTUM_RETURN_SPEED * dt);
    }
    _camera->setFrustumAngle(newFrustumAngle);
}

void World::setCity()
{
    _city->repositionBuildings(_superhero->getPosition(), _superhero->getForward());
}

/**
 * Gets the start position
 */
vec3 World::getStartPosition() const
{
    return _startPosition;
}
