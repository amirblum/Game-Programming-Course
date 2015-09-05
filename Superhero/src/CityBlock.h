//
//  CityBlock.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__CityBlock__
#define __CGP_Superhero__CityBlock__

#include <stdio.h>
#include "SceneNode.h"
#include "Mesh.h"

class CityBlock : public SceneNode {
public:
    CityBlock(vec3 position, float buildingWidth, float roadWidth);
    virtual ~CityBlock();
};

#endif /* defined(__CGP_Superhero__CityBlock__) */
