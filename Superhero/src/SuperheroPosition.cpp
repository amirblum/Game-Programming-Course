//
//  SuperheroPosition.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
using namespace glm;

#include "SuperheroPosition.h"

#define CAMERA_DISTANCE (2.5f)

SuperheroPosition::SuperheroPosition(City *city, Superhero *superhero) :
_city(city), _superhero(superhero)
{
}

SuperheroPosition::~SuperheroPosition()
{}

void SuperheroPosition::update(float dt)
{
    vec3 oldPosition = _superhero->getWorldPosition();
    vec3 newPosition = oldPosition + _superhero->getVelocity() * _superhero->getVelocityMultiplier() * dt;
    float radius = _superhero->getRadius();
    
    float blockSize = BUILDING_WIDTH + ROAD_WIDTH;
    float halfRoad = ROAD_WIDTH * 0.5f;
    
    int row = floor((newPosition.z + halfRoad) / blockSize);
    int col = floor((newPosition.x + halfRoad) / blockSize);
    
//    std::cout << "Row: " << row << " Col: " << col << std::endl;
        
    float buildingLeft = col * blockSize;
    float buildingRight = buildingLeft + BUILDING_WIDTH;
    buildingLeft -= radius;
    buildingRight += radius;
    
    float buildingBottom = row * blockSize;
    float buildingTop = buildingBottom + BUILDING_WIDTH;
    buildingBottom -= radius;
    buildingTop += radius;
    
    bool collided = false;
    
    if (newPosition.z > buildingBottom && newPosition.z < buildingTop) {
        if (oldPosition.x <= buildingLeft && newPosition.x > buildingLeft) {
            newPosition.x = buildingLeft;
            collided = true;
        } else if (oldPosition.x >= buildingRight && newPosition.x < buildingRight) {
            newPosition.x = buildingRight;
            collided = true;
        }
    }
    
    if (newPosition.x > buildingLeft && newPosition.x < buildingRight) {
        if (oldPosition.z <= buildingBottom && newPosition.z > buildingBottom) {
            newPosition.z = buildingBottom;
            collided = true;
        } else if (oldPosition.z >= buildingTop && newPosition.z < buildingTop) {
            newPosition.z = buildingTop;
            collided = true;
        }
    }
    
    vec3 positionDiff = newPosition - oldPosition;
    _superhero->setPosition(_superhero->getPosition() + positionDiff);
    
    if (collided) {
        _superhero->collideWithBuilding();
    }
}
