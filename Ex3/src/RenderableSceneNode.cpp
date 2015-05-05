//
//  RenderableSceneNode.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "RenderableSceneNode.h"

RenderableSceneNode::RenderableSceneNode(std::string programName, vec3 position, quat rotation, vec3 scale) :
SceneNode(position, rotation, scale)
{
    _renderComponent = new RenderComponent(programName);
}

RenderableSceneNode::~RenderableSceneNode()
{
    delete _renderComponent;
}

void RenderableSceneNode::render()
{
    updateUniforms();
    
    _renderComponent->render(_worldTransform);
}

void RenderableSceneNode::updateUniforms() {}