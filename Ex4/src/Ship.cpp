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
#include "AsteroidRigidBody.h"
#include "BlackHole.h"
#include "Beacon.h"

#include <iostream>

static const std::string SHIP_MESH = "assets/vipermk2/vipermk2_LW7.obj";
//static const std::string SHIP_MESH = "assets/simonship/simonship.obj";
//static const std::string SHIP_MESH = "assets/serenity/serenity.3ds";
static const std::string DRADIS_SOUND = "assets/sounds/dradis.wav";
static const std::string THRUSTERS_SOUND = "assets/sounds/thrusters.wav";
static const std::string EXPLOSION_SOUND = "assets/sounds/explosion.wav";
static const std::string WARNING_SOUND = "assets/sounds/warning.wav";
static const std::string WIN_SOUND = "assets/sounds/win.wav";

#define SHIP_MASS (0.5f)
#define TILT_SPEED (pi<float>() / 4.0f)
#define ROLL_SPEED (pi<float>() / 2.0f)
#define ACCELERATION_FORCE (10.0f)
#define DAMPENING_FORCE    (0.005f)
#define MAX_VELOCITY (300.0f)
#define MAX_HEALTH (5)

/**
 * Ship constructor
 */
Ship::Ship(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
RigidBody(position, vec3(0.0f), radius, SHIP_MASS, false),
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
    
    // Activate shield
    {
        vec3 shieldPosition = vec3(0.0f, 0.0f, -0.05f);
        _shield = new Shield(shieldPosition, radius);
        addChild(_shield);
    }
    
    // Initialize sounds
    {
        SoundManager &soundManager = SoundManager::Instance();
        _dradisSound = soundManager.loadSound(DRADIS_SOUND, true);
        _thrustersSound = soundManager.loadSound(THRUSTERS_SOUND, true);
        _explosionSound = soundManager.loadSound(EXPLOSION_SOUND);
        _warningSound = soundManager.loadSound(WARNING_SOUND, true);
        _winSound = soundManager.loadSound(WIN_SOUND);
        
        // Play the dradis sound on init
        soundManager.playSound(_dradisSound);
    }
}

/**
 * Deconstructor
 */
Ship::~Ship()
{
    // Sounds
    SoundManager &soundManager = SoundManager::Instance();
    soundManager.releaseSound(_dradisSound);
    soundManager.releaseSound(_thrustersSound);
    soundManager.releaseSound(_explosionSound);
    soundManager.releaseSound(_warningSound);
}

void Ship::fixedUpdate(float dt)
{
    GameState &state = GameState::Instance();
    if (state.gameStarted && !state.gameOver && !state.gameWon) {
        
        InputManager &input = InputManager::Instance();
        
        // Velocity hack. Basically this retains the forward velocity even when
        //changing directions. Not realistic physics AT ALL...but feels better XD
//        vec3 prevVelocity = _physics.getVelocity();
//        float velocityStrength = length(prevVelocity);
//        _physics.applyForce(-prevVelocity);
//        _physics.applyForce(_forward * velocityStrength);
        
        // Accelerating
        SoundManager &soundManager = SoundManager::Instance();
        if (input.isPressed(KEY_ACTION)) {
            accelerate(ACCELERATION_FORCE);
//            _physics.applyForce(_forward * ACCELERATION_FORCE);
            for (unsigned int i = 0; i < 3; ++i) {
                for (unsigned int j = 0; j < 100; ++j) {
                    _thrusters[i]->emit();
                }
            }
            soundManager.playSound(_thrustersSound);
        } else if (_physics.isMoving()) {
//            _physics.applyForce(-_physics.getVelocity() * DAMPENING_FORCE);
            soundManager.stopSound(_thrustersSound);
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
    } else if (_physics.isMoving()) {
//        _physics.applyForce(-_physics.getVelocity() * DAMPENING_FORCE);
    }
    
    // Update position
//    vec3 velocity = _physics.getVelocity();
//    setPosition(_position + velocity * dt);
    
}

/**
 * Perform non-physics related updates
 */
void Ship::update(float dt)
{
    GameState &state = GameState::Instance();
    if (state.gameStarted && !state.gameWon) {
        // Update position
        setPosition(_physics.getPosition());
    }
    
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
    _physics.applyForce(_forward * force);
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

void Ship::onCollision(RigidBody *collided)
{
    // Asteroid
    AsteroidRigidBody *asteroid = dynamic_cast<AsteroidRigidBody*>(collided);
    if (asteroid != nullptr) {
        collideWithAsteroid(asteroid);
        return;
    }
    
    // Black Hole
    BlackHole *blackHole = dynamic_cast<BlackHole*>(collided);
    if (blackHole != nullptr) {
        collideWithBlackHole(blackHole);
        return;
    }
    
    // Beacon
    Beacon *beacon = dynamic_cast<Beacon*>(collided);
    if (beacon != nullptr) {
        collideWithBeacon(beacon);
        return;
    }
}

void Ship::collideWithAsteroid(AsteroidRigidBody *asteroid)
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
    
    if (newHealth >= 0) {
        _healthBar->setCurrentUnits(newHealth);
        if (newHealth == 0) {
            // Gameover
            die();
        } else {
            // Explosion
            vec3 toAsteroid = asteroid->getPhysics().getPosition() - getWorldPosition();
            vec3 explosionOffset = normalize(toAsteroid) * _radius;
            generateExplosion(explosionOffset, 20);
        }
    }
}

void Ship::collideWithBlackHole(BlackHole *blackHole)
{
    die();
}

void Ship::collideWithBeacon(Beacon *beacon)
{
    win();
}

void Ship::generateExplosion(vec3 position, unsigned int explosionSize)
{
    ExplosionParticleSystem *newExplosion = new ExplosionParticleSystem(explosionSize, position);
    newExplosion->toggleRotationInvariance();
    newExplosion->toggleScaleInvariance();
    addChild(newExplosion, 1);
    
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

/**
 * Return the direction of movement
 */
vec3 Ship::getMovementDirection()
{
    vec3 velocity = _physics.getVelocity();
    if (length(velocity) < 0.01f) {
        return velocity;
    } else {
        return normalize(velocity);
    }
}

float Ship::getSpeed()
{
    return length(_physics.getVelocity());
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
    generateExplosion(vec3(0.0f), 2000);
    removeChild(_shield);
    
    SoundManager &soundManager = SoundManager::Instance();
    soundManager.stopSound(_dradisSound);
    soundManager.stopSound(_thrustersSound);
    soundManager.stopSound(_warningSound);
}

void Ship::win()
{
    SoundManager::Instance().playSound(_winSound);
    GameState::Instance().gameWon = true;
    
    SoundManager &soundManager = SoundManager::Instance();
    soundManager.stopSound(_dradisSound);
    soundManager.stopSound(_thrustersSound);
    soundManager.stopSound(_warningSound);
}

bool Ship::isDead()
{
    return _healthBar->getCurrentUnits() == 0;
}
