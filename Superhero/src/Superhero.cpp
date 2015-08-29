//
//  Superhero.cpp
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Superhero.h"
#include "InputManager.h"
#include "GameState.h"

#include <iostream>

//static const std::string SUPERHERO_MESH = "assets/vipermk2/vipermk2_LW7.obj";
//static const std::string FLYING_SOUND = "assets/sounds/thrusters.wav";
//static const std::string SUPERHERO_MESH = "assets/buzz/Creature.obj";
static const std::string SUPERHERO_MESH = "assets/superman/Superman.obj";
static const std::string FLYING_SOUND = "assets/sounds/thrusters.wav";


#define ACCELERATION_SPEED (30.0f)
#define TURN_SPEED (pi<float>() / 2.0f)
#define MAX_VELOCITY (300.0f)
#define MAX_HEALTH (5)

/**
 * Superhero constructor
 */
Superhero::Superhero(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
_dead(false),
_forward(FORWARD_VECTOR),
_radius(radius)
{
    // Initialize mesh
    {
        // Scale for viper
        vec3 meshPosition = vec3(0.0f);
//        quat meshRotation = rotate(quat(vec3(0.0f)), pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
        // buzz
//        quat meshRotation = quat(vec3(pi<float>() / 8.0f, 0.0f, pi<float>()));
        quat meshRotation = quat(vec3(pi<float>() / 3.5f, 0.0f, 0.0f));
        // superman
        
        
        vec3 meshScale = vec3(1.0f);

        // buzz
//        _mesh = new Mesh(SUPERHERO_MESH, 0.05f, meshPosition, meshRotation, meshScale);
        
        // superman
        _mesh = new Mesh(SUPERHERO_MESH, 1.0f, meshPosition, meshRotation, meshScale);
        addChild(_mesh);
    }
    
    // Construct healthbar
    {
        vec3 healthPosition = vec3(0.0f, -0.3f, -1.225f);
        quat healthRotation = quat(vec3(0.0f));
        vec3 healthScale = vec3(0.2f, 0.06f, 1.0f);
        _healthBar = new HealthBar(MAX_HEALTH, healthPosition, healthRotation, healthScale);
        addChild(_healthBar);
    }
    
    // Initialize sounds
    {
        SoundManager &soundManager = SoundManager::Instance();
        _flyingSound = soundManager.loadSound(FLYING_SOUND, true);
    }
}

/**
 * Deconstructor
 */
Superhero::~Superhero()
{
    // Sounds
    SoundManager &soundManager = SoundManager::Instance();
    soundManager.releaseSound(_flyingSound);
}

void Superhero::update(float dt)
{
    GameState &state = GameState::Instance();
    if (state.winState == STARTED) {
        InputManager &input = InputManager::Instance();
        SoundManager &soundManager = SoundManager::Instance();
        
        // Turning
        if (input.isPressed(KEY_LEFT))
        {
            turn(true, dt);
        }
        else if (input.isPressed(KEY_RIGHT))
        {
            turn(false, dt);
        }
        
        // Accelerating
        if (input.isPressed(KEY_UP))
        {
            accelerate(true, dt);
            soundManager.playSound(_flyingSound);
        }
        else if (input.isPressed(KEY_DOWN))
        {
            accelerate(false, dt);
            soundManager.playSound(_flyingSound);
        } else {
            dampen(dt);
            soundManager.stopSound(_flyingSound);
        }
    }
    
    // Clamp velocity
    if (length(_velocity) > MAX_VELOCITY) {
        _velocity = normalize(_velocity) * MAX_VELOCITY;
    }
    
    // Update position
    setPosition(_position + _velocity * dt);
    
}

/**
 * Accelerate in the _forward direction
 */
void Superhero::accelerate(bool forward, float dt)
{
    _velocity += _forward * ACCELERATION_SPEED * (forward ? 1.0f : -1.0f) * dt;
}

void Superhero::dampen(float dt)
{
    _velocity *= 0.5f * dt;
    if (length(_velocity) < epsilon<float>()) {
        _velocity = vec3(0.0f);
    }
}

/**
 * Turn superhero right/left
 */
void Superhero::turn(bool left, float dt)
{
    float angle = TURN_SPEED * (left ? 1.0f : -1.0f) * dt;
    setRotation(rotate(_rotation, angle, UP_VECTOR));
    
    _forward = _rotation * FORWARD_VECTOR;
}

void Superhero::onCollision()
{
    // Building
//    AsteroidRigidBody *asteroid = dynamic_cast<AsteroidRigidBody*>(collided);
//    if (asteroid != nullptr) {
//        collideWithAsteroid(asteroid);
//        return;
//    }
}

void Superhero::collideWithBuilding()
{
    // Health
    int health = _healthBar->getCurrentUnits();
    int newHealth = health - 1;
    
    // Sound
//    alSourcePlay(_explosionSound);
    if (newHealth == 1) {
//        alSourcePlay(_warningSound);
//        alSourcei(_warningSound, AL_LOOPING, AL_TRUE);
    }
    
    if (newHealth >= 0) {
        _healthBar->setCurrentUnits(newHealth);
        if (newHealth == 0) {
            // Gameover
            die();
        }
    }
}

/**
 * Return the forward-facing vector
 */
vec3 Superhero::getForward()
{
    return _forward;
}

/**
 * Return the direction of movement
 */
vec3 Superhero::getMovementDirection()
{
    if (length(_velocity) < 0.01f) {
        return _velocity;
    } else {
        return normalize(_velocity);
    }
}

float Superhero::getSpeed()
{
    return length(_velocity);
}

float Superhero::getMaxSpeed()
{
    return MAX_VELOCITY;
}

float Superhero::getRadius()
{
    return _radius;
}

void Superhero::die()
{
    if (_dead) return;
    _dead = true;
    _healthBar->setCurrentUnits(0);
    
    SoundManager &soundManager = SoundManager::Instance();
    soundManager.stopSound(_flyingSound);
}

void Superhero::win()
{
    GameState::Instance().winState = WON;
    
    SoundManager &soundManager = SoundManager::Instance();
    soundManager.stopSound(_flyingSound);
}

bool Superhero::isDead()
{
    return _healthBar->getCurrentUnits() == 0;
}
