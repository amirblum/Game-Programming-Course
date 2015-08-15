//
//  BoundingBox.h
//  CGP-Superhero
//
//  Created by Amir Blum on 8/15/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__BoundingBox__
#define __CGP_Superhero__BoundingBox__

// GLM headers
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
using namespace glm;

#include <stdio.h>

class BoundingBox {
private:
    vec3 _min, _max;
public:
    BoundingBox();
    BoundingBox(vec3 min, vec3 max);
    ~BoundingBox();
    
    void includePoint(vec3 point);
    float minX();
    float maxX();
    float minY();
    float maxY();
    float minZ();
    float maxZ();
};

#endif /* defined(__CGP_Superhero__BoundingBox__) */
