//
//  World.cpp
//  CGP-Ex4
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
    
//    DummyObject *dummy = new DummyObject(vec3(-1.0f, 0.0f, 1.0f), quat(vec3(0.0f)), vec3(1.0f));
//    addChild(dummy);
    
    // Skybox (draw last to save on fragments that already have info in them)
    SkyBox *skybox = new SkyBox();
    
    Beacon *beacon = new Beacon(BEACON_POS, quat(vec3(0.0f)), vec3(1.0f), 300.0f);
    addChild(beacon);
    
    // Set up black holes (After skybox so that transparency will work)
    vec3 hole1Pos = vec3(800.0f, 20.0f, 2000.0f);
    float hole1size = 400.0f;
    BlackHole *hole1 = new BlackHole(hole1Pos, hole1size);
    
    vec3 hole2Pos = vec3(-350.0f, -200.0f, 2500.0f);
    float hole2size = 250.0f;
    BlackHole *hole2 = new BlackHole(hole2Pos, hole2size);
    
    // Set up asteroids
    quat asteroids1Rotation = quat(vec3(0.0f, 2.0f, 0.0f));
    AsteroidParticleSystem *asteroids1 = new AsteroidParticleSystem(hole1Pos, asteroids1Rotation, 100, hole1size * 2.0f);
    
    quat asteroids2Rotation = quat(vec3(0.7f, 0.0f, 0.0f));
    AsteroidParticleSystem *asteroids2 = new AsteroidParticleSystem(hole2Pos, asteroids2Rotation, 100, hole2size * 3.0f);
    
    // Ship
    _ship = new Ship(vec3(0.0f), quat(vec3(0.0f)), vec3(1.0f, 1.0f, 1.0f), /*radius*/1.6f);
    
    // Add children
    {
        addChild(asteroids1);
        addChild(asteroids2);
        addChild(skybox);
        // Things that are rendered after skybox will be able to use transparency with the skybox
        addChild(hole1);
        addChild(hole2);
        addChild(_ship);
    }
    
    // Scripts
    addScript(new CameraScripts(camera, _ship, skybox));
    
    // Background music
    _backgroundMusic = SoundManager::Instance().loadSound(BACKGROUND_MUSIC, true);
    
    GameState::Instance().reset();
}

World::~World()
{
    SoundManager::Instance().releaseSound(_backgroundMusic);
}

void World::fixedUpdate(float dt)
{
    PhysicsSimulation::Instance().fixedUpdate(dt);
}

/**
 * Any updates to world objects should happen here
 */
void World::update(float dt)
{
    GameState &state = GameState::Instance();
    
    if (state.gameStarted) {
        if (!_started) {
            SoundManager::Instance().playSound(_backgroundMusic);
            _started = true;
        }
        
        if (!state.gameOver && InputManager::Instance().isPressedFirstTime(KEY_GAME_OVER)) {
            _ship->die();
        }
        
        if (!state.gameOver && _ship->isDead()) {
            Overlay *gameOverText = new Overlay(GAME_OVER_TEXTURE);
            addChild(gameOverText);
            state.gameOver = true;
        }
        
        if (state.gameWon) {
            state.gameWon = false; // So that we only perform this once
            state.gameOver = true;
            
            Overlay *youWinText = new Overlay(YOU_WIN_TEXTURE);
            addChild(youWinText);
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