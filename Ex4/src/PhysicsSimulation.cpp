//
//  PhysicsSimulation.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 5/31/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "PhysicsSimulation.h"

PhysicsSimulation::PhysicsSimulation() :
_forceSources(), _objects()
{
    
}

void PhysicsSimulation::addObject(RigidBody *object, bool isSource)
{
    // Add all sources to this object
    for (RigidBody *source : _forceSources) {
        object->getPhysics().addForceSource(&source->getPhysics());
    }
    
    if (isSource) {
        // Add as a source to all other objects
        for (RigidBody *otherObject : _objects) {
            otherObject->getPhysics().addForceSource(&object->getPhysics());
        }
        
        // Add to sources vector
        _forceSources.push_back(object);
    }
    
    // Add to objects vector
    _objects.push_back(object);
}

void PhysicsSimulation::removeObject(RigidBody *object)
{
    // Search in force sources
    auto sourceIter = std::find(_forceSources.begin(), _forceSources.end(), object);
    if (sourceIter != _forceSources.end()) {
        _objects.erase(sourceIter);
        // Remove the source from all other objects
        for (RigidBody *otherObject : _objects) {
            otherObject->getPhysics().removeForceSource(&object->getPhysics());
        }
    }
    
    // Remove from objects vector
    auto iter = std::find(_objects.begin(), _objects.end(), object);
    if (iter != _objects.end()) {
        _objects.erase(iter);
    }
}

void PhysicsSimulation::fixedUpdate(float dt)
{
    for (RigidBody *object : _forceSources) {
        object->physicsUpdate(dt);
    }
    
    for (RigidBody *object : _objects) {
        object->physicsUpdate(dt);
    }
    
    checkCollisions();
}

void PhysicsSimulation::checkCollisions()
{
    for (unsigned int i = 0; i < _objects.size() - 1; ++i) {
        for (unsigned int j = i + 1; j < _objects.size(); ++j) {
            RigidBody *object1 = _objects[i];
            RigidBody *object2 = _objects[j];
            
            vec3 centerToCenter = object1->getPhysics().getPosition() - object2->getPhysics().getPosition();
            float distance = length(centerToCenter);
            
            float radiiSum = object1->getRadius() + object2->getRadius();
            if (distance <= radiiSum) {
                float previousDistance = length(object1->getPhysics().getPreviousPosition() - object2->getPhysics().getPreviousPosition());
                
                if (previousDistance > radiiSum) {
                    // This is the first collision
                    vec3 collisionNormal = normalize(centerToCenter);
                    object1->handleCollision(object2, collisionNormal);
                    object2->handleCollision(object1, collisionNormal);
                }
            }

        }
    }
}