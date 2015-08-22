//
//  Road.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__Road__
#define __CGP_Superhero__Road__

#include <stdio.h>
#include "SceneNode.h"
#include "Mesh.h"

class Road : public SceneNode {
private:
    Mesh *_mesh;
    float _height;
    
public:
    Road(std::string filename,
         float unitConversion,
         vec3 position, quat rotation);
    virtual ~Road();
    
    void stretchToFill(vec3 size);
};

#endif /* defined(__CGP_Superhero__Road__) */
