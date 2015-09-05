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

static const std::string roadMesh = "assets/road/road1.obj";
static const std::string intersectionMesh = "assets/intersection/road3.obj";

City::City(int gridWidth, int gridLength) :
SceneNode(),
_gridWidth(gridWidth), _gridLength(gridLength),
_buildingsGrid(gridWidth * gridLength)
{
    float gridSpacing = BUILDING_WIDTH + ROAD_WIDTH;
    float halfBuildingWidth = BUILDING_WIDTH / 2.0f;
    float halfStreetWidth = ROAD_WIDTH / 2.0f;
    float roadOffset = halfBuildingWidth + halfStreetWidth;
    
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridLength; ++j) {
            float buildingX = gridSpacing * i + halfBuildingWidth;
            float buildingZ = gridSpacing * j + halfBuildingWidth;
            
            // Building
            Building *building = new Building(vec3(buildingX, 0.0f, buildingZ));
            building->stretchToFill(vec3(BUILDING_WIDTH, -1.0f, BUILDING_WIDTH));
            _buildingsGrid[i * gridLength + j] = building;
            addChild(building);
            
            // Roads
            {
                // Upper road
                float upperRoadX = buildingX;
                float upperRoadZ = buildingZ + roadOffset;
                Road *upperRoad = new Road(roadMesh, 0.01f, vec3(upperRoadX, 0.0f, upperRoadZ), quat(vec3(0.0f, pi<float>() / 2.0f, 0.0f)));
                upperRoad->stretchToFill(vec3(ROAD_WIDTH, -1.0f, BUILDING_WIDTH));
                addChild(upperRoad);
                
                // Side road
                float sideRoadX = buildingX + roadOffset;
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

int City::getGridWidth()
{
    return _gridWidth;
}

int City::getGridLength()
{
    return _gridLength;
}

Building* City::getClosestBuilding(vec3 position)
{
    float blockSize = BUILDING_WIDTH + ROAD_WIDTH;
//    float cityWidth = _gridWidth * blockSize;
//    float cityLength = _gridLength * blockSize;

    int row = position.z / blockSize;
    row = clamp(row, 0, _gridLength);
    int col = position.x / blockSize;
    col = clamp(col, 0, _gridWidth);
    
    std::cout << "Closest building at row: " << row << " col: " << col << std::endl;
    
    return _buildingsGrid[col * _gridLength + row];
}

void City::repositionBuildings(vec3 pos, vec3 dir)
{
    float blockSize = BUILDING_WIDTH + ROAD_WIDTH;

    int row = pos.z / blockSize;
    int col = pos.x / blockSize;
    
    float firstRow = row - _gridLength / 2;
    float lastRow = row + _gridLength / 2;
    float firstCol = col - _gridWidth / 2;
    float lastCol = col + _gridWidth / 2;
    
    float gridSpacing = BUILDING_WIDTH + ROAD_WIDTH;
    float halfBuildingWidth = BUILDING_WIDTH / 2.0f;
    float halfStreetWidth = ROAD_WIDTH / 2.0f;
    float roadOffset = halfBuildingWidth + halfStreetWidth;
    for (int i = firstRow; i < lastRow + 1; ++i) {
        for (int j = firstCol; j < lastCol + 1; ++j) {
            float buildingX = gridSpacing * j + halfBuildingWidth;
            float buildingZ = gridSpacing * i + halfBuildingWidth;
            
            int gridRowIndex = i % _gridLength;
            if (gridRowIndex < 0) {
                gridRowIndex += _gridLength;
            }
            int gridColIndex = j % _gridWidth;
            if (gridColIndex < 0) {
                gridColIndex += _gridWidth;
            }
            int gridIndex = gridRowIndex * _gridWidth + gridColIndex;
            vec3 newBlockPosition = _buildingsGrid[gridIndex]->getPosition();
            newBlockPosition.x = buildingX;
            newBlockPosition.z = buildingZ;
            _buildingsGrid[gridIndex]->setPosition(newBlockPosition);
        }
    }
}