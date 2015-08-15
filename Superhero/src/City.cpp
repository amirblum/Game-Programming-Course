//
//  City.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "City.h"
#include "Building.h"
#include "RandUtils.h"

#define BUILDING_MIN_HEIGHT (20.0f)
#define BUILDING_MAX_HEIGHT (50.0f)
#define BUILDING_WIDTH (20.0f)
#define STREET_WIDTH (10.0f)

City::City(int gridWidth, int gridHeight) :
SceneNode(),
_gridWidth(gridWidth), _gridHeight(gridHeight),
_buildingsGrid(gridWidth * gridHeight)
{
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            float gridSpacing = BUILDING_WIDTH + STREET_WIDTH;
            float buildingX = gridSpacing * i;
            float buildingY = gridSpacing * j;
            
            // Create random height
            float height = randutils::randomRange(BUILDING_MIN_HEIGHT, BUILDING_MAX_HEIGHT);
            
            _buildingsGrid[i] = new Building(vec3(buildingX, buildingY, 0.0f), BUILDING_WIDTH, height);
            addChild(_buildingsGrid[i]);
        }
    }
}

City::~City()
{
    
}

