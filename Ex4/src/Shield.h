//
//  Shield.h
//  CGP-Ex4
//
//  Created by Amir Blum on 6/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__Shield__
#define __CGP_Ex4__Shield__

#include <stdio.h>
#include "RenderableSceneNode.h"

class Shield : public RenderableSceneNode {
//private:
//    float _radius;
public:
    Shield(vec3 pos, float radius);
    virtual ~Shield();
};

#endif /* defined(__CGP_Ex4__Shield__) */
