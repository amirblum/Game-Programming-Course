//
//  Ship.h
//  cg-projects
//
//  The main character of our game
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__Ship__
#define __CGP_Ex4__Ship__

#include "SceneNode.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "HealthBar.h"
#include "Shield.h"
#include "ThrusterParticleSystem.h"
#include "ExplosionParticleSystem.h"
#include "SoundManager.h"

// Rigidbodies
#include "AsteroidRigidBody.h"
#include "BlackHole.h"
#include "Beacon.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Ship : public SceneNode, public RigidBody {
private:
    // Sounds
    ALuint _dradisSound, _thrustersSound, _explosionSound, _warningSound, _winSound;
    
    // Components
    Mesh *_mesh;
    
    // Convenient info
    vec3 _forward;
    vec3 _right;
    float _radius;
    
    // Children
    HealthBar *_healthBar;
    Shield *_shield;
    ThrusterParticleSystem* _thrusters[3];
    std::vector<ExplosionParticleSystem*> _explosions;
    
    // Movement
    void accelerate(float force);
    void tilt(float angle);
    void twist(float angle);
    void generateExplosion(vec3 position, unsigned int explosionSize);
    
    // Collisions
    void collideWithAsteroid(AsteroidRigidBody *asteroid);
    void collideWithBlackHole(BlackHole *blackHole);
    void collideWithBeacon(Beacon *beacon);
public:
    Ship(vec3 position, quat rotation, vec3 scale, float radius);
    virtual ~Ship();
    
    virtual void fixedUpdate(float dt);
    virtual void update(float dt);
    
    virtual void onCollision(RigidBody *collided);
    
    // Getters
    vec3 getForward();
    vec3 getRight();
    vec3 getMovementDirection();
    float getSpeed();
    float getMaxSpeed();
    float getRadius();
    
    void die();
    void win();
    bool isDead();
};

#endif /* defined(__CGP_Ex4__Ship__) */
