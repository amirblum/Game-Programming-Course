//
//  Building.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Building.h"
#include "RandUtils.h"

static const std::string texturesLocation = "assets/buildings/";
static const std::string meshes[] = {
    "building1/building1.obj",
    "building2/building2.obj",
    "building3/building3.obj",
    "building4/building4.obj"
};

Building::Building(vec3 position) :
SceneNode(position)
{
    // Initialize random mesh
    {
        float unitConversion;
        int buildingIndex = randutils::randomRange(0, 4);
//        int buildingIndex = 2;
        
        std::string shipMesh = texturesLocation + "/" + meshes[buildingIndex];
        switch (buildingIndex) {
            case 0:
                unitConversion = 0.0002f;
                break;
            case 1:
                unitConversion = 0.01f;
                break;
            case 2:
                unitConversion = 0.005f;
                break;
            case 3:
                unitConversion = 0.005f;
                break;
            default:
                unitConversion = 1.0f;
                break;
        }
        
        _mesh = new Mesh(shipMesh, unitConversion, vec3(0.0f), quat(vec3(0.0f)), vec3(1.0f));
        addChild(_mesh);
    }
    
    // Set height
    {
        BoundingBox bb = _mesh->getBoundingBox();
        _height = bb.maxY() - bb.minY();
        
        vec3 myPosition = getPosition();
        myPosition.y = _height / 2;
        setPosition(myPosition);
    }
}

Building::~Building()
{
    
}

float Building::getHeight() {
    return _height;
}