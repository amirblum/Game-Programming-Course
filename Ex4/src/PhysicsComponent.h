//
//  PhysicsComponent.h
//  CGP-Ex4
//
//  Component for representing physics in the world
//
//  Created by Amir Blum on 4/28/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__PhysicsComponent__
#define __CGP_Ex4__PhysicsComponent__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <stdio.h>
#include <vector>

class PhysicsComponent {
private:
    float _mass;
    float _inverseMass;
    bool _isStatic;
    
    std::vector<PhysicsComponent*> _externalForceSources;
    vec3 _internalForce;
    
    struct State {
        vec3 x; // position
        vec3 v; // velocity
    };
    
    struct Derivative {
        vec3 dx; // dx/dt = velocity
        vec3 dv; // dv/dt = aceleration
    };
    
    State _previousState, _currentState;
    
    Derivative evaluate(float dt, const Derivative &d);
    vec3 acceleration(const State &state);
    void rungeKutta4(float dt);
    
public:
    PhysicsComponent(vec3 initialPos, float mass);
    virtual ~PhysicsComponent();
    
    // Getters
    vec3 getPosition() const;
    vec3 getPreviousPosition() const;
    vec3 getVelocity() const;
    float getMass() const;
    float getInverseMass() const;
    
    // State changers (should only be called in a fixed update)
    void update(float dt);
    
    void setStatic(bool isStatic);
    void applyForce(vec3 force);
    void addForceSource(PhysicsComponent *source);
    void removeForceSource(PhysicsComponent *source);
    void handleCollision(const PhysicsComponent &other, const vec3 &collisionNormal);
    
    // State queryers
    bool isMoving() const;
};

#endif /* defined(__CGP_Ex4__PhysicsComponent__) */
