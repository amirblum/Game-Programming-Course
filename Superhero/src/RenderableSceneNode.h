//
//  RenderableSceneNode.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__RenderableSceneNode__
#define __CGP_Superhero__RenderableSceneNode__

#include <stdio.h>
#include "SceneNode.h"
#include "RenderComponent.h"

class RenderableSceneNode : public SceneNode {
protected:
    RenderComponent *_renderComponent;
    
    virtual void preRender();
    virtual void postRender();
public:
    RenderableSceneNode(std::string programName,
                        vec3 position = vec3(0.0f),
                        quat rotation = quat(vec3(0.0f)),
                        vec3 scale = vec3(1.0f));
    virtual ~RenderableSceneNode();
    
    virtual void render();
};

#endif /* defined(__CGP_Superhero__RenderableSceneNode__) */
