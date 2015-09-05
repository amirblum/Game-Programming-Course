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
    
    int firstRow, firstCol;
    int lastRow, lastCol;
    
    float angle = atan2f(dir.z, dir.x);
    if (angle < 0) {
        angle += 2.0f * pi<float>();
    }
    
//    std::cout << "Looking at angle " << angle << std::endl;
    
    float halfPi = pi<float>() / 2.0f;
    float quadrant2Begin = halfPi / 2.0f;
    float quadrant3Begin = quadrant2Begin + halfPi;
    float quadrant4Begin = quadrant3Begin + halfPi;
    float quadrant1Begin = quadrant4Begin + halfPi;
    
    if (angle > quadrant2Begin && angle <= quadrant3Begin) {
//        std::cout << "Looking forward" << std::endl;
        firstRow = row - 1;
        lastRow = row + _gridLength - 1;
        firstCol = col - _gridWidth / 2;
        lastCol = col + _gridWidth / 2 + 1;
    } else if (angle > quadrant3Begin && angle <= quadrant4Begin) {
//        std::cout << "Looking left" << std::endl;
        firstRow = row - _gridLength / 2;
        lastRow = row + _gridLength / 2 + 1;
        firstCol = col - _gridWidth + 2;
        lastCol = col + 2;
    } else if (angle > quadrant4Begin && angle <= quadrant1Begin) {
//        std::cout << "Looking back" << std::endl;
        firstRow = row - _gridLength + 2;
        lastRow = row + 2;
        firstCol = col - _gridWidth / 2;
        lastCol = col + _gridWidth / 2 + 1;
    } else {
//        std::cout << "Looking right" << std::endl;
        firstRow = row - _gridWidth / 2;
        lastRow = row + _gridWidth / 2 + 1;
        firstCol = col - 1;
        lastCol = col + _gridWidth - 1;
    }
    
    float gridSpacing = BUILDING_WIDTH + ROAD_WIDTH;
    float halfBuildingWidth = BUILDING_WIDTH / 2.0f;
    float halfStreetWidth = ROAD_WIDTH / 2.0f;
    float roadOffset = halfBuildingWidth + halfStreetWidth;
    for (int i = firstRow; i < lastRow; ++i) {
        for (int j = firstCol; j < lastCol; ++j) {
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