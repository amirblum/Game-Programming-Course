//
//  Ship.h
//  cg-projects
//
//  The main character of our game
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __ex3__Ship__
#define __ex3__Ship__

#include "SceneNode.h"
#include "Mesh.h"
#include "PhysicsComponent.h"
#include "HealthBar.h"
#include "SkyBox.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Ship : public SceneNode {
private:
    // Components
    Mesh *_mesh;
    PhysicsComponent *_physicsComponent;
    
    // Convenient info
    vec3 _forward;
    vec3 _right;
    float _radius;
    
    // Children
    HealthBar *_healthBar;
    
    // Movement
    void accelerate(float force);
    void tilt(float angle);
    void twist(float angle);
public:
    Ship(vec3 position, quat rotation, vec3 scale, float radius);
    virtual ~Ship();
    
    virtual void render();
    virtual void update(float dt);
    
    void collide();
    
    // Getters
    vec3 getForward();
    vec3 getRight();
    float getRadius();
};

#endif /* defined(__ex3__Ship__) */
