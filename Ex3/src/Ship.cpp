//
//  Ship.cpp
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ShaderIO.h"
#include "Ship.h"
#include "InputManager.h"
#include "GameState.h"

#include <iostream>

static const std::string SHIP_MESH = "assets/vipermk2/vipermk2_LW7.obj";
//static const std::string SHIP_MESH = "assets/simonship/simonship.obj";
//static const std::string SHIP_MESH = "assets/serenity/serenity.3ds";
static const std::string DRADIS_SOUND = "assets/sounds/dradis.wav";
static const std::string THRUSTERS_SOUND = "assets/sounds/thrusters.wav";
static const std::string EXPLOSION_SOUND = "assets/sounds/explosion.wav";
static const std::string WARNING_SOUND = "assets/sounds/warning.wav";

#define TILT_SPEED (pi<float>() / 4.0f)
#define ROLL_SPEED (pi<float>() / 2.0f)
#define ACCELERATION_FORCE (1.0f)
#define DAMPENING_FORCE    (0.005f)
#define MAX_VELOCITY (300.0f)
#define MAX_HEALTH (5)

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
_forward(FORWARD_VECTOR), _right(RIGHT_VECTOR),
_radius(radius)
{
    // Initialize mesh
    {
        // Scale for serenity
//        vec3 meshPosition = vec3(0.0f);
//        quat meshRotation = quat(vec3(0.0f));
//        vec3 meshScale = vec3(0.02f);
        
         // Scale for simonship
//        vec3 meshPosition = vec3(0.0f);
//        quat meshRotation = quat(vec3(0.0f, 0.0f, 0.0f));
//        vec3 meshScale = vec3(0.005f);
        
        // Scale for viper
        vec3 meshPosition = vec3(0.0f);
        quat meshRotation = rotate(quat(vec3(0.0f)), pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
        vec3 meshScale = vec3(1.0f);
        
        _mesh = new Mesh(SHIP_MESH, meshPosition, meshRotation, meshScale);
        addChild(_mesh);
    }
    
    // Construct thrusters
    {
        float thrusterPositions[] = {
            -0.25f, -0.08f, -1.4f,
            0.25f, -0.08f, -1.4f,
            0.0f, 0.0775f, -1.4f
        };
        
        for (unsigned int i = 0; i < 3; ++i) {
            // Position
            vec3 thrusterPosition;
            thrusterPosition.x = thrusterPositions[i * 3];
            thrusterPosition.y = thrusterPositions[i * 3 + 1];
            thrusterPosition.z = thrusterPositions[i * 3 + 2];
            
            // Rotation
            quat thrusterRotation = quat(vec3(0.0f));
            
            // Scale
            vec3 thrusterScale = vec3(1.0f, 1.0f, 1.5f);
            
            _thrusters[i] = new ThrusterParticleSystem(20000, 0.1,
                                                       thrusterPosition,
                                                       thrusterRotation,
                                                       thrusterScale);
            
            addChild(_thrusters[i]);
        }
    }
    
    // Construct healthbar
    {
        vec3 healthPosition = vec3(0.0f, -0.3f, -1.225f);
        quat healthRotation = quat(vec3(0.0f));
        vec3 healthScale = vec3(0.2f, 0.06f, 1.0f);
        _healthBar = new HealthBar(MAX_HEALTH, healthPosition, healthRotation, healthScale);
        addChild(_healthBar);
    }
    
    // Initialize sounds (this should be moved to a sound manager later)
    {
        ALenum alutError;
        
        // Dradis
        _dradisBuffer = alutCreateBufferFromFile(DRADIS_SOUND.c_str());
        
        alutError = alutGetError();
        if (alutError != AL_NO_ERROR) {
            std::cout << "Error loading dradis sound: " << alutGetErrorString(alutError) << std::endl;
        }
        
        alGenSources(1, &_dradisSound);
        alSourcei(_dradisSound, AL_BUFFER, _dradisBuffer);
        alSourcei(_dradisSound, AL_LOOPING, true);
        alSourcePlay(_dradisSound);
        
        // Thrusters
        _thrustersBuffer = alutCreateBufferFromFile(THRUSTERS_SOUND.c_str());
        
        alutError = alutGetError();
        if (alutError != AL_NO_ERROR) {
            std::cout << "Error loading thrusters sound: " << alutGetErrorString(alutError) << std::endl;
        }
        
        alGenSources(1, &_thrustersSound);
        alSourcei(_thrustersSound, AL_BUFFER, _thrustersBuffer);
        alSourcei(_thrustersSound, AL_LOOPING, AL_TRUE);
        
        // Explosion
        _explosionBuffer = alutCreateBufferFromFile(EXPLOSION_SOUND.c_str());
        alutError = alutGetError();
        if (alutError != AL_NO_ERROR) {
            std::cout << "Error loading explosion sound: " << alutGetErrorString(alutError) << std::endl;
        }
        
        alGenSources(1, &_explosionSound);
        alSourcei(_explosionSound, AL_BUFFER, _explosionBuffer);
        
        // Warning
        _warningBuffer = alutCreateBufferFromFile(WARNING_SOUND.c_str());
        alutError = alutGetError();
        if (alutError != AL_NO_ERROR) {
            std::cout << "Error loading warning sound: " << alutGetErrorString(alutError) << std::endl;
        }
        
        alGenSources(1, &_warningSound);
        alSourcei(_warningSound, AL_BUFFER, _warningBuffer);
        alSourcei(_warningSound, AL_LOOPING, AL_TRUE);
    }
    
    // Initialize components
    _physicsComponent = new PhysicsComponent(MAX_VELOCITY);
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
    delete _physicsComponent;
    alSourceStop(_dradisSound);
    alSourceStop(_warningSound);
    alDeleteSources(1, &_dradisSound);
    alDeleteBuffers(1, &_dradisBuffer);
    alDeleteSources(1, &_thrustersSound);
    alDeleteBuffers(1, &_thrustersBuffer);
    alDeleteSources(1, &_explosionSound);
    alDeleteBuffers(1, &_explosionBuffer);
    alDeleteSources(1, &_warningSound);
    alDeleteBuffers(1, &_warningBuffer);
}

void Ship::render()
{
}

void Ship::update(float dt)
{
    GameState &state = GameState::Instance();
    if (state.gameStarted && !state.gameOver) {
        
        InputManager &input = InputManager::Instance();
        
        // Velocity hack. Basically this retains the forward velocity even when
        //changing directions. Not realistic physics AT ALL...but feels better XD
        vec3 prevVelocity = _physicsComponent->getVelocity();
        float velocityStrength = length(prevVelocity);
        _physicsComponent->applyForce(-prevVelocity);
        _physicsComponent->applyForce(_forward * velocityStrength);
        
        // Accelerating
        ALint thrustersSoundState;
        if (input.isPressed(KEY_ACTION)) {
            _physicsComponent->applyForce(_forward * ACCELERATION_FORCE);
            for (unsigned int i = 0; i < 3; ++i) {
                for (unsigned int j = 0; j < 400; ++j) {
                    _thrusters[i]->emit();
                }
            }
            alGetSourcei(_thrustersSound, AL_SOURCE_STATE, &thrustersSoundState);
            if (thrustersSoundState != AL_PLAYING) {
                alSourcePlay(_thrustersSound);
            }
        } else if (_physicsComponent->isMoving()) {
            _physicsComponent->applyForce(-_physicsComponent->getVelocity() * DAMPENING_FORCE);
            alGetSourcei(_thrustersSound, AL_SOURCE_STATE, &thrustersSoundState);
            if (thrustersSoundState == AL_PLAYING) {
                alSourceStop(_thrustersSound);
            }
        }
        
        // Tilting
        if (input.isPressed(KEY_UP))
        {
            tilt(TILT_SPEED * dt);
        }
        else if (input.isPressed(KEY_DOWN))
        {
            tilt(-TILT_SPEED * dt);
        }
        
        // Twisting
        if (input.isPressed(KEY_LEFT))
        {
            twist(-ROLL_SPEED * dt);
        }
        else if (input.isPressed(KEY_RIGHT))
        {
            twist(ROLL_SPEED * dt);
        }
    } else if (_physicsComponent->isMoving()) {
        _physicsComponent->applyForce(-_physicsComponent->getVelocity() * DAMPENING_FORCE);
    }
    
    // Update position
    vec3 velocity = _physicsComponent->getVelocity();
    setPosition(_position + velocity * dt);
    
    // Update explosions
    {
        std::vector<unsigned int> explosionsToRemove;
        for (unsigned int i = 0; i < _explosions.size(); ++i) {
            if (_explosions[i]->isDead()) {
                explosionsToRemove.push_back(i);
                removeChild(_explosions[i]);
            }
        }
        for (unsigned int i : explosionsToRemove) {
            _explosions.erase(_explosions.begin() + i);
        }
    }
}

/**
 * Accelerate in the _forward direction
 */
void Ship::accelerate(float force)
{
    _physicsComponent->applyForce(_forward * force);
}

/**
 * Tilt ship up/down
 */
void Ship::tilt(float angle)
{
    setRotation(rotate(_rotation, angle, RIGHT_VECTOR));
    
    // No need to recompute right vector, as it didn't change.
    _forward = _rotation * FORWARD_VECTOR;
}

/**
 * Twist ship right/left
 */
void Ship::twist(float angle)
{
    setRotation(rotate(_rotation, angle, FORWARD_VECTOR));
    
    // No need to recompute forward vector, as it didn't change.
    _right = _rotation * RIGHT_VECTOR;
}

void Ship::collide()
{
    // Health
    int health = _healthBar->getCurrentUnits();
    int newHealth = health - 1;
    
    // Sound
    alSourcePlay(_explosionSound);
    if (newHealth == 1) {
        alSourcePlay(_warningSound);
        alSourcei(_warningSound, AL_LOOPING, AL_TRUE);
    }
    
    // Gameover
    if (newHealth >= 0) {
        _healthBar->setCurrentUnits(newHealth);
        if (newHealth == 0) {
            std::cout << "Game Over!" << std::endl;
            die();
        } else {
            // Explosion
            generateExplosion(20);
        }
    }
    
}

void Ship::generateExplosion(unsigned int explosionSize)
{
    ExplosionParticleSystem *newExplosion = new ExplosionParticleSystem(explosionSize);
    newExplosion->toggleRotationInvariance();
    newExplosion->toggleScaleInvariance();
    addChild(newExplosion);
    
    _explosions.push_back(newExplosion);
}

/**
 * Return the forwar-facing vector
 */
vec3 Ship::getForward()
{
    return _forward;
}

/**
 * Return the right-facing vector
 */
vec3 Ship::getRight()
{
    return _right;
}

float Ship::getSpeed()
{
    return length(_physicsComponent->getVelocity());
}

float Ship::getMaxSpeed()
{
    return MAX_VELOCITY;
}

float Ship::getRadius()
{
    return _radius;
}

void Ship::die()
{
    _healthBar->setCurrentUnits(0);
    alSourceStop(_dradisSound);
    alSourceStop(_thrustersSound);
    generateExplosion(2000);
}

bool Ship::isDead()
{
    return _healthBar->getCurrentUnits() == 0;
}
