//
//  RenderableSceneNode.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__RenderableSceneNode__
#define __CGP_Ex3__RenderableSceneNode__

#include <stdio.h>
#include "SceneNode.h"
#include "RenderComponent.h"

class RenderableSceneNode : public SceneNode {
protected:
    RenderComponent *_renderComponent;
    
public:
    RenderableSceneNode(std::string programName,
                        vec3 position = vec3(0.0f),
                        quat rotation = quat(vec3(0.0f)),
                        vec3 scale = vec3(1.0f));
    virtual ~RenderableSceneNode();
    
    virtual void render();
    virtual void updateUniforms();
};

#endif /* defined(__CGP_Ex3__RenderableSceneNode__) */
