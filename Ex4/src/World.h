//
//  World.h
//  CGP-Ex4
//
//  Root node of the scene graph. Creates the nodes/initializes scripts
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__World__
#define __CGP_Ex4__World__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
using namespace glm;

#include "SceneNode.h"
#include "Ship.h"
#include "SoundManager.h"
#include "PhysicsSimulation.h"

class World : public SceneNode {
private:
    ALuint _backgroundMusic;
    
    vec3 _startPosition;
    bool _started;
    
    Ship *_ship;
    PhysicsSimulation *_physicsSimulation;
    
public:
    World();
    virtual ~World();
    virtual void fixedUpdate(float dt);
    virtual void update(float dt);
    
    // Getters
    vec3 getStartPosition() const;
};

#endif /* defined(__CGP_Ex4__World__) */
