//
//  Beacon.h
//  CGP-Ex4
//
//  Created by Amir Blum on 6/4/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__Beacon__
#define __CGP_Ex4__Beacon__

#include <stdio.h>
#include "SceneNode.h"
#include "RigidBody.h"
#include "Mesh.h"

class Beacon : public SceneNode, public RigidBody {
private:
    Mesh *_mesh;
    RenderComponent _compass;
public:
    Beacon(vec3 position, quat rotation, vec3 scale, float radius);
    virtual ~Beacon();
    
    virtual void render();
};

#endif /* defined(__CGP_Ex4__Beacon__) */

