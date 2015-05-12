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

static const std::string SHIP_MESH = "assets/vipermk2.lwo";
static const std::string EXPLOSION_SOUND = "assets/explosion.wav";

#define TILT_SPEED (pi<float>() / 4.0f)
#define ROLL_SPEED (pi<float>() / 2.0f)
#define ACCELERATION_FORCE (0.3f)
#define DAMPENING_FORCE    (0.01f)
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
        vec3 meshPosition = vec3(0.0f);
        quat meshRotation = rotate(quat(vec3(0.0f)), pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
        vec3 meshScale = vec3(1.0f);
        _mesh = new Mesh(SHIP_MESH, meshPosition, meshRotation, meshScale);
        addChild(_mesh);
    }
    
    // Construct healthbar
    {
        vec3 healthPosition = vec3(0.0f, 1.0f, 0.0f);
        quat healthRotation = quat(vec3(0.0f));
        vec3 healthScale = vec3(1.0f, 0.1f, 1.0f);
        _healthBar = new HealthBar(MAX_HEALTH, healthPosition, healthRotation, healthScale);
        addChild(_healthBar);
    }
    
    // Initialize sounds (this should be moved to a sound manager later)
    {
        ALuint soundBuffer;
        soundBuffer = alutCreateBufferFromFile(EXPLOSION_SOUND.c_str());
        ALenum alutError = alutGetError();
        if (alutError != AL_NO_ERROR) {
            std::cout << "Error loading explosion sound: " << alutGetErrorString(alutError) << std::endl;
        }
        
        alGenSources(1, &_soundSrc);
        alSourcei(_soundSrc, AL_BUFFER, soundBuffer);
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
    delete _healthBar;
}

void Ship::render()
{
}

void Ship::update(float dt)
{
    if (!GameState::Instance().gameOver) {
        
        InputManager &input = InputManager::Instance();
        
        // Velocity hack. Basically this retains the forward velocity even when
        //changing directions. Not realistic physics AT ALL...but feels better XD
        vec3 prevVelocity = _physicsComponent->getVelocity();
        float velocityStrength = length(prevVelocity);
        _physicsComponent->applyForce(-prevVelocity);
        _physicsComponent->applyForce(_forward * velocityStrength);
        
        // Accelerating
        if (input.isPressed(KEY_ACTION)) {
            _physicsComponent->applyForce(_forward * ACCELERATION_FORCE);
        } else if (_physicsComponent->isMoving()) {
            _physicsComponent->applyForce(-_physicsComponent->getVelocity() * DAMPENING_FORCE);
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
    alSourcePlay(_soundSrc);
    
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
    generateExplosion(2000);
}

bool Ship::isDead()
{
    return _healthBar->getCurrentUnits() == 0;
}
