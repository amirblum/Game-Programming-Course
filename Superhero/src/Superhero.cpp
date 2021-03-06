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

static const std::string SUPERHERO_MESH = "assets/buzz/Creature.obj";
//static const std::string SUPERHERO_MESH = "assets/superman/Superman.obj";
static const std::string FLYING_SOUND = "assets/sounds/thrusters.wav";
static const std::string EXPLOSION_SOUND = "assets/sounds/explosion.wav";


#define ACCELERATION_SPEED (15.0f)
#define MAX_TURN_SPEED (pi<float>() * 1.25f)
#define MIN_TURN_SPEED (pi<float>() / 2.0f)
#define HOLD_TO_TURN_TIME (0.15f)
#define STRAFE_SPEED (5.0f)
#define MAX_VELOCITY (50.0f)
#define BOOSTING_VELOCITY (150.0f)
#define DAMPEN_PERCENT (0.5f)
#define BOOST_DAMPEN_PERCENT (0.2f)
#define MAX_HEALTH (5)
#define MAX_BLUR (10.0f)
#define INITIAL_BLUR_TIME (0.2f)
#define BLUR_DECREASE (15.0f)

/**
 * Superhero constructor
 */
Superhero::Superhero(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
_dead(false),
_forward(FORWARD_VECTOR),
_radius(radius),
_velocity(0.0f), _velocityMultiplier(1.0f), _rotationMulitplier(1.0f),
_holdTurnTime(0.0f), _movingForward(false), _initialBlurTime(INITIAL_BLUR_TIME)
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
        _explosionSound = soundManager.loadSound(EXPLOSION_SOUND, false);
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
    if (state.winState != STARTED) {
        return;
    }
    
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
    if (state.boostState != BOOSTING && input.isPressed(KEY_UP))
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
    
    // Clamp velocity
    if (state.boostState != BOOSTING && getSpeed() > MAX_VELOCITY) {
        setSpeed(MAX_VELOCITY);
    }
    
    // Set blur amount
    if (state.boostState == BOOSTING) {
        float expectedBlur = MAX_BLUR * ((getSpeed() - MAX_VELOCITY) / (BOOSTING_VELOCITY - MAX_VELOCITY));
        if (_initialBlurTime < INITIAL_BLUR_TIME) {
            _initialBlurTime += dt;
            state.blurSteps = min(MAX_BLUR * (_initialBlurTime / INITIAL_BLUR_TIME), expectedBlur);
            return;
        }
        state.blurSteps = expectedBlur;
    } else {
        _initialBlurTime = 0.0f;
        
        state.blurSteps = max(state.blurSteps - BLUR_DECREASE * dt, 0.0f);
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
//    std::cout << "Accelerating at speed " << newSpeed << std::endl;
    setSpeed(newSpeed);
}

void Superhero::dampen(float dt)
{
    float newSpeed = getSpeed();
    float dampenPercent = (GameState::Instance().boostState == BOOSTING) ? BOOST_DAMPEN_PERCENT : DAMPEN_PERCENT;
    float dampenAmount = newSpeed * dampenPercent * dt;
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
    
    if (!_movingForward) {
        newSpeed = -newSpeed;
    }
    
    setRotation(rotate(_rotation, angle * _rotationMulitplier, UP_VECTOR));
    
    _forward = _rotation * FORWARD_VECTOR;
    setSpeed(newSpeed);
}

void Superhero::setSpeed(float speed) {
    _velocity = _forward * speed;
    _movingForward = speed > 0;
    
    GameState &state = GameState::Instance();
    if (state.boostState == BOOSTING && speed <= MAX_VELOCITY) {
        state.boostState = NONE;
    }
}

void Superhero::collideWithBuilding()
{
    if (_collided) {
        return;
    }
    
    _collided = true;
    
    // Slow down
    float newSpeed = MAX_VELOCITY * 0.5f;
    if (!_movingForward) {
        newSpeed = -newSpeed;
    }
    setSpeed(newSpeed);
    GameState::Instance().boostState = NONE;
    
    // Health
    int health = _healthBar->getCurrentUnits();
    int newHealth = health - 1;
    
    // Sound
    alSourcePlay(_explosionSound);
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

void Superhero::uncollide()
{
    _collided = false;
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

float Superhero::getVelocityMultiplier()
{
    return _velocityMultiplier;
}

float Superhero::getSpeed()
{
    return length(_velocity);
}

float Superhero::getMaxSpeed()
{
    return MAX_VELOCITY;
}

float Superhero::getMaxBoostSpeed()
{
    return BOOSTING_VELOCITY;
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
    _velocityMultiplier = 0.2f;
    _rotationMulitplier = 0.2f;
}

void Superhero::unslow() {
    _velocityMultiplier = 1.0f;
    _rotationMulitplier = 1.0f;
}

void Superhero::boost(float boostPercent) {
    _velocityMultiplier = 1.0f;
    _rotationMulitplier = 1.0f;
    setSpeed(mix(MAX_VELOCITY, BOOSTING_VELOCITY, boostPercent));
}

bool Superhero::isDead()
{
    return _healthBar->getCurrentUnits() == 0;
}
