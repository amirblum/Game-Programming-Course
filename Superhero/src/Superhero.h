//
//  Superhero.h
//  cg-projects
//
//  The main character of our game
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__Superhero__
#define __CGP_Superhero__Superhero__

#include "SceneNode.h"
#include "Mesh.h"
#include "HealthBar.h"
#include "SoundManager.h"

// Rigidbodies forward decleration
class AsteroidRigidBody;
class BlackHole;
class Beacon;

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Superhero : public SceneNode {
private:
    bool _dead;
    
    // Sounds
    ALuint _flyingSound;
    
    // Components
    Mesh *_mesh;
    
    // Movement/collision info
    vec3 _forward;
    float _radius;
    vec3 _velocity;
    float _holdTurnTime;
    
    // Children
    HealthBar *_healthBar;
    
    // Movement
    void accelerate(bool forward, float dt);
    void dampen(float dt);
    void turn(bool left, float dt);
    
    // Collisions
    void collideWithBuilding();
    
public:
    Superhero(vec3 position, quat rotation, vec3 scale, float radius);
    virtual ~Superhero();
    
    virtual void update(float dt);
    
    virtual void onCollision();
    
    // Getters
    vec3 getForward();
    vec3 getMovementDirection();
    float getSpeed();
    float getMaxSpeed();
    float getRadius();
    
    void die();
    void win();
    bool isDead();
};

#endif /* defined(__CGP_Superhero__Superhero__) */
