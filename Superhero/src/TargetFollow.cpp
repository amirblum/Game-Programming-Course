//
//  TargetFollow.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "TargetFollow.h"

TargetFollow::TargetFollow(SceneNode *object, SceneNode *target) :
_object(object), _target(target)
{
}

TargetFollow::~TargetFollow()
{}

void TargetFollow::update(float dt)
{
    vec3 targetPosition = _target->getPosition();
    _object->setPosition(targetPosition);
}