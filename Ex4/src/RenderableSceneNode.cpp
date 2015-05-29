//
//  RenderableSceneNode.cpp
//  CGP-Ex4
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
    preRender();
    _renderComponent->render(_worldTransform);
    postRender();
}

void RenderableSceneNode::preRender() {}
void RenderableSceneNode::postRender() {}