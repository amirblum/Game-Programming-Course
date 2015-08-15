//
//  TargetFollow.h
//  CGP-Superhero
//
//  Script to lock a scene node to a target
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__TargetFollow__
#define __CGP_Superhero__TargetFollow__

#include <stdio.h>
#include "Script.h"
#include "SceneNode.h"

class TargetFollow : public Script {
private:
    SceneNode *_object;
    SceneNode *_target;
    
public:
    TargetFollow(SceneNode *object, SceneNode *target);
    virtual ~TargetFollow();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Superhero__TargetFollow__) */
