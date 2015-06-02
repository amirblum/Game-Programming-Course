//
//  PhysicsSimulation.h
//  CGP-Ex4
//
//  Created by Amir Blum on 5/31/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__PhysicsSimulation__
#define __CGP_Ex4__PhysicsSimulation__

#include <stdio.h>
#include <vector>
#include "RigidBody.h"

class PhysicsSimulation {
private:
    // Singleton private instantiation
    PhysicsSimulation();
    PhysicsSimulation(PhysicsSimulation const&);
    void operator=(PhysicsSimulation const&);
    
    // Private functions
    void checkCollisions();
    
    std::vector<RigidBody *> _forceSources;
    std::vector<RigidBody *> _objects;
    
public:
    // Singleton implementation
    static PhysicsSimulation& Instance() {
        static PhysicsSimulation instance;
        
        return instance;
    }
    
    void addObject(RigidBody *object, bool isSource);
    void removeObject(RigidBody *object);
    
    void fixedUpdate(float dt);
};

#endif /* defined(__CGP_Ex4__PhysicsSimulation__) */
