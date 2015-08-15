//
//  City.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__City__
#define __CGP_Superhero__City__

#include <stdio.h>
#include "SceneNode.h"

class Building;

class City : public SceneNode {
private:
    int _gridWidth, _gridHeight;
    std::vector<Building*> _buildingsGrid;
    
public:
    City(int gridWidth, int gridHeight);
    virtual ~City();
    
    int getGridWidth();
    int getGridHeight();
};

#endif /* defined(__CGP_Superhero__City__) */
