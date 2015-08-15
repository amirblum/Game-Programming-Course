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
#include "SceneNode.h"
#include "Mesh.h"

class Building : public SceneNode {
private:
    Mesh *_mesh;
    float _height;
    
public:
    Building(vec3 position);
    virtual ~Building();
    
    float getHeight();
};

#endif /* defined(__CGP_Superhero__Building__) */
