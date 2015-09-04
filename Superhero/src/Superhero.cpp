//
//  Superhero.cpp
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Superhero.h"
#include "Building.h"
#include "InputManager.h"
#include "GameState.h"

#include <iostream>

static const std::string SUPERHERO_MESH = "assets/buzz/Creature.obj";
//static const std::string SUPERHERO_MESH = "assets/superman/Superman.obj";
static const std::string FLYING_SOUND = "assets/sounds/thrusters.wav";


#define ACCELERATION_SPEED (15.0f)
#define MAX_TURN_SPEED (pi<float>() * 1.25f)
#define MIN_TURN_SPEED (pi<float>() / 2.0f)
#define HOLD_TO_TURN_TIME (0.15f)
#define STRAFE_SPEED (5.0f)
#define MAX_VELOCITY (50.0f)
#define BOOSTING_VELOCITY (100.0f)
#define DAMPEN_PERCENT (0.5f)
#define MAX_HEALTH (5)

/**
 * Superhero constructor
 */
Superhero::Superhero(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
_dead(false),
_forward(FORWARD_VECTOR),
_radius(radius),
_velocity(0.0f), _velocityMultiplier(1.0f), _rotationMulitplier(1.0f),
_holdTurnTime(0.0f), _movingForward(false), _boosting(false)
{
    // Initialize mesh
    {
        vec3 meshPosition = vec3(0.0f);
//        quat meshRotation = rotate(quat(vec3(0.0f)), pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
        // buzz
        quat meshRotation = quat(vec3(pi<float>() / 8.0f, 0.0f, pi<float>()));
        // superman
//        quat meshRotation = quat(vec3(pi<float>() / 3.5f, 0.0f, 0.0f));
        
        
        vec3 meshScale = vec3(1.0f);

        // buzz
        _mesh = new Mesh(SUPERHERO_MESH, 0.05f, meshPosition, meshRotation, meshScale);
        
        // superman
//        _mesh = new Mesh(SUPERHERO_MESH, 1.0f, meshPosition, meshRotation, meshScale);
        addChild(_mesh);
    }
    
    // Construct healthbar
    {
        vec3 healthPosition = vec3(0.33f, -0.54f, -1.0f);
        quat healthRotation = quat(vec3(-0.1f, 0.0f, -1.2f));
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
        } else {
            _holdTurnTime = 0;
        }
        
        // Accelerating
        if (!_boosting && input.isPressed(KEY_UP))
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
    if (!_boosting && getSpeed() > MAX_VELOCITY) {
        setSpeed(MAX_VELOCITY);
    }
}

/**
 * Accelerate in the _forward direction
 */
void Superhero::accelerate(bool forward, float dt)
{
    float speedModifier = _movingForward ? 1.0f : -1.0f;
    float forwardModifier = forward ? 1.0f : -1.0f;
    
    
    float newSpeed = getSpeed() * speedModifier + ACCELERATION_SPEED * forwardModifier * dt;
    std::cout << "Accelerating at speed " << newSpeed << std::endl;
    setSpeed(newSpeed);
}

void Superhero::dampen(float dt)
{
    float newSpeed = getSpeed();
    float dampenAmount = newSpeed * DAMPEN_PERCENT * dt;
    if (_movingForward) {
        newSpeed = max(newSpeed - dampenAmount, 0.0f);
    } else {
        newSpeed = min(-newSpeed + dampenAmount, 0.0f);
    }
    setSpeed(newSpeed);
}

/**
 * Turn superhero right/left
 */
void Superhero::turn(bool left, float dt)
{
    float currentSpeed = getSpeed();
    float relativeSpeed = currentSpeed / MAX_VELOCITY;
    float turnSpeed = mix(MIN_TURN_SPEED, MAX_TURN_SPEED, relativeSpeed);
    float angle = turnSpeed * (left ? 1.0f : -1.0f) * dt;
    
    float newSpeed = currentSpeed;
    if (currentSpeed > MAX_VELOCITY * 0.5f) {
        if (_holdTurnTime > HOLD_TO_TURN_TIME) {
            newSpeed *= mix(1.0f, 0.5f, relativeSpeed);
        } else {
            angle *= mix(0.5f, 1.0f, _holdTurnTime / HOLD_TO_TURN_TIME);
            _holdTurnTime += dt;
        }
    }
    
    if (InputManager::Instance().isPressed(KEY_DOWN)) {
        newSpeed = -newSpeed;
    }
    
    setRotation(rotate(_rotation, angle * _rotationMulitplier, UP_VECTOR));
    
    _forward = _rotation * FORWARD_VECTOR;
    setSpeed(newSpeed);
}

void Superhero::setSpeed(float speed) {
    _velocity = _forward * speed * _velocityMultiplier;
    _movingForward = speed > 0;
    if (speed <= MAX_VELOCITY) {
        _boosting = false;
    }
}

void Superhero::collideWithBuilding()
{
    if (getSpeed() < MAX_VELOCITY * 0.5f) {
        return;
    }
    
    // Slow down
    setSpeed(MAX_VELOCITY * 0.5f);
    _boosting = false;
    
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

vec3 Superhero::getVelocity()
{
    return _velocity;
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

void Superhero::slowDown() {
    _velocityMultiplier = 0.9f;
    _rotationMulitplier = 0.2f;
}

void Superhero::boost() {
    _velocityMultiplier = 1.0f;
    _rotationMulitplier = 1.0f;
    setSpeed(BOOSTING_VELOCITY);
    _boosting = true;
}

bool Superhero::isBoosting()
{
    return _boosting;
}

bool Superhero::isDead()
{
    return _healthBar->getCurrentUnits() == 0;
}
