//
//  Ship.h
//  cg-projects
//
//  The main character of our game
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex3__Ship__
#define __ex3__Ship__

#include "SceneNode.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Ship : public SceneNode {
private:
    // Components
    RenderComponent _renderComponent;
    PhysicsComponent _physicsComponent;
    
    // Convenient info
    vec3 _forward;
    vec3 _right;
public:
    Ship(vec3 position, quat rotation, vec3 scale);
    virtual ~Ship();
    
    virtual void update(float dt);
    virtual void render();
    
    void accelerate(float force);
    void tilt(float angle);
    void twist(float angle);
    
    // Getters
    vec3 getForward();
    vec3 getRight();
};

#endif /* defined(__ex3__Ship__) */
