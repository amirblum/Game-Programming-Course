//
//  BlackHole.h
//  CGP-Ex4
//
//  Created by Amir Blum on 6/1/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__BlackHole__
#define __CGP_Ex4__BlackHole__

#include <stdio.h>
#include "RigidBody.h"
#include "PhysicsSimulation.h"
#include "RenderableSceneNode.h"

class BlackHole : public RenderableSceneNode, public RigidBody {
private:
    float _size;
    float _spinAmount;
    vec2 _right;
protected:
    virtual void preRender();
public:
    BlackHole(vec3 position, float size);
    virtual ~BlackHole();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Ex4__BlackHole__) */
