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

#define BUILDING_WIDTH (20.0f)
#define ROAD_WIDTH (10.0f)

class Building;

class City : public SceneNode {
private:
    int _gridWidth, _gridLength;
    std::vector<Building*> _buildingsGrid;
    
public:
    City(int gridWidth, int gridlength);
    virtual ~City();
    
    int getGridWidth();
    int getGridLength();
    
    Building* getClosestBuilding(vec3 position);
};

#endif /* defined(__CGP_Superhero__City__) */
