//
//  DummyObject.h
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __ex3__DummyObject__
#define __ex3__DummyObject__

#include "SceneNode.h"
#include "RenderComponent.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class DummyObject : public SceneNode {
private:
    RenderComponent _renderComponent;

// Personal
public:
    DummyObject(vec3 position, quat rotation, vec3 scale);
    virtual ~DummyObject();
    
    virtual void update(float dt);
    virtual void render();
};

#endif /* defined(__ex3__DummyObject__) */
