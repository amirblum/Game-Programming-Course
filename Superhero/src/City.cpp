//
//  City.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "City.h"
#include "Building.h"
#include "Road.h"
#include "RandUtils.h"

#define BUILDING_MIN_HEIGHT (20.0f)
#define BUILDING_MAX_HEIGHT (50.0f)
#define BUILDING_WIDTH (20.0f)
#define ROAD_WIDTH (10.0f)

static const std::string roadMesh = "assets/road/road1.obj";
static const std::string intersectionMesh = "assets/intersection/road3.obj";

City::City(int gridWidth, int gridHeight) :
SceneNode(),
_gridWidth(gridWidth), _gridHeight(gridHeight),
_buildingsGrid(gridWidth * gridHeight)
{
    float gridSpacing = BUILDING_WIDTH + ROAD_WIDTH;
    float halfBuildingWidth = BUILDING_WIDTH / 2.0f;
    float halfStreetWidth = ROAD_WIDTH / 2.0f;
    float streetOffset = halfBuildingWidth + halfStreetWidth;
    
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            float buildingX = gridSpacing * i;
            float buildingZ = gridSpacing * j;
            
            // Building
            Building *building = new Building(vec3(buildingX, 0.0f, buildingZ));
            building->stretchToFill(vec3(BUILDING_WIDTH, -1.0f, BUILDING_WIDTH));
            _buildingsGrid[i] = building;
            addChild(building);
            
            // Roads
            {
                // Upper road
                float upperRoadX = buildingX;
                float upperRoadZ = buildingZ + streetOffset;
                Road *upperRoad = new Road(roadMesh, 0.01f, vec3(upperRoadX, 0.0f, upperRoadZ), quat(vec3(0.0f, pi<float>() / 2.0f, 0.0f)));
                upperRoad->stretchToFill(vec3(ROAD_WIDTH, -1.0f, BUILDING_WIDTH));
                addChild(upperRoad);
                
                // Side road
                float sideRoadX = buildingX + streetOffset;
                float sideRoadZ = buildingZ;
                Road *sideRoad = new Road(roadMesh, 0.01f, vec3(sideRoadX, 0.0f, sideRoadZ), quat(vec3(0.0f)));
                sideRoad->stretchToFill(vec3(ROAD_WIDTH, -1.0f, BUILDING_WIDTH));
                addChild(sideRoad);
                
                // Intersection
                float intersectionX = sideRoadX;
                float intersectionZ = upperRoadZ;
                Road *intersection = new Road(intersectionMesh, 0.01f, vec3(intersectionX, 0.0f, intersectionZ), quat(vec3(0.0f)));
                intersection->stretchToFill(vec3(ROAD_WIDTH, -1.0f, ROAD_WIDTH));
                addChild(intersection);
            }
        }
    }
}

City::~City()
{
    
}

