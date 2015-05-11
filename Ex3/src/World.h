//
//  World.h
//  CGP-Ex3
//
//  Root node of the scene graph. Creates the nodes/initializes scripts
//
//  Created by Amir Blum on 4/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__World__
#define __CGP_Ex3__World__

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
#include "CameraFollow.h"

class World : public SceneNode {
private:
    vec3 _startPosition;
    
    Ship *_ship;
    
    CameraFollow *_cameraFollow;
    
public:
    World();
    virtual ~World();
    virtual void update(float dt);
    
    // Getters
    vec3 getStartPosition() const;
};

#endif /* defined(__CGP_Ex3__World__) */
