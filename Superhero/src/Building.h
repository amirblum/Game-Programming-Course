//
//  Building.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__Building__
#define __CGP_Superhero__Building__

#include <stdio.h>
#include "RenderableSceneNode.h"

class Building : public RenderableSceneNode {
private:
    float _width, _height;
    
public:
    Building(vec3 position, float width, float height);
    virtual ~Building();
    
    float getHeight();
};

#endif /* defined(__CGP_Superhero__Building__) */
