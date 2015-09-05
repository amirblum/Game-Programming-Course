//
//  City.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "City.h"
#include "CityBlock.h"
#include "RandUtils.h"

#define BUILDING_MIN_HEIGHT (20.0f)
#define BUILDING_MAX_HEIGHT (50.0f)

City::City(int gridWidth, int gridLength) :
SceneNode(),
_gridWidth(gridWidth), _gridLength(gridLength),
_blockGrid(gridWidth * gridLength)
{
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridLength; ++j) {
            // Block
            CityBlock *block = new CityBlock(vec3(0.0f), BUILDING_WIDTH, ROAD_WIDTH);
            _blockGrid[i * gridLength + j] = block;
            addChild(block);
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

//Building* City::getClosestBuilding(vec3 position)
//{
//    float blockSize = BUILDING_WIDTH + ROAD_WIDTH;
////    float cityWidth = _gridWidth * blockSize;
////    float cityLength = _gridLength * blockSize;
//
//    int row = position.z / blockSize;
//    row = clamp(row, 0, _gridLength);
//    int col = position.x / blockSize;
//    col = clamp(col, 0, _gridWidth);
//    
//    std::cout << "Closest building at row: " << row << " col: " << col << std::endl;
//    
//    return _buildingsGrid[col * _gridLength + row];
//}

void City::repositionBuildings(vec3 pos, vec3 dir)
{
    float blockSize = BUILDING_WIDTH + ROAD_WIDTH;

    int row = pos.z / blockSize;
    int col = pos.x / blockSize;
    
    float firstRow = row - _gridLength / 2;
    float lastRow = row + _gridLength / 2;
    float firstCol = col - _gridWidth / 2;
    float lastCol = col + _gridWidth / 2;
    
    float halfBuildingWidth = BUILDING_WIDTH / 2.0f;
    for (int i = firstRow; i < lastRow + 1; ++i) {
        for (int j = firstCol; j < lastCol + 1; ++j) {
            float buildingX = blockSize * j + halfBuildingWidth;
            float buildingZ = blockSize * i + halfBuildingWidth;
            
            int gridRowIndex = i % _gridLength;
            if (gridRowIndex < 0) {
                gridRowIndex += _gridLength;
            }
            int gridColIndex = j % _gridWidth;
            if (gridColIndex < 0) {
                gridColIndex += _gridWidth;
            }
            int gridIndex = gridRowIndex * _gridWidth + gridColIndex;
            vec3 newBlockPosition = _blockGrid[gridIndex]->getPosition();
            newBlockPosition.x = buildingX;
            newBlockPosition.z = buildingZ;
            _blockGrid[gridIndex]->setPosition(newBlockPosition);
        }
    }
}