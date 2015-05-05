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

#include "RenderableSceneNode.h"
#include "PhysicsComponent.h"
#include "HealthBar.h"
#include "SkyBox.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Ship : public RenderableSceneNode {
private:
    // Components
    PhysicsComponent *_physicsComponent;
    
    // Convenient info
    vec3 _forward;
    vec3 _right;
    
    // Children
    HealthBar *_healthBar;
    
public:
    Ship(vec3 position, quat rotation, vec3 scale);
    virtual ~Ship();
    
    virtual void update(float dt);
    
    void accelerate(float force);
    void tilt(float angle);
    void twist(float angle);
    
    // Getters
    vec3 getForward();
    vec3 getRight();
};

#endif /* defined(__ex3__Ship__) */
