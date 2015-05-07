//
//  PhysicsComponent.h
//  CGP-Ex3
//
//  Component for representing physics in the world
//
//  Created by Amir Blum on 4/28/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__PhysicsComponent__
#define __CGP_Ex3__PhysicsComponent__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <stdio.h>

class PhysicsComponent {
private:
    vec3 _velocity;
    vec3 _acceleration;
    
    float _maxVelocity;
    
public:
    PhysicsComponent(float maxVelocity);
    virtual ~PhysicsComponent();
    
    // Getters
    vec3 getVelocity();
    
    // State changers
    void update(float dt);
    void applyForce(vec3 force);
    bool isMoving();
};

#endif /* defined(__CGP_Ex3__PhysicsComponent__) */
