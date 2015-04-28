//
//  World.h
//  CGP-Ex3
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

#include <glm/glm.hpp>
using namespace glm;

#include "SceneNode.h"
#include "Controller.h"
#include "CameraFollow.h"

class World : public SceneNode {
private:
    vec3 _startPosition;
    Camera *_camera;
    Controller *_controller;
    CameraFollow *_cameraFollow;
    
public:
    World();
    virtual ~World();
    virtual void update(float dt);
    virtual void render();
    
    // Getters
    vec3 getStartPosition() const;
};

#endif /* defined(__CGP_Ex3__World__) */
