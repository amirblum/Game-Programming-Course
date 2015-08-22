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
//        int buildingIndex = randutils::randomRange(0, 4);
        int buildingIndex = 0;
        
        std::string buildingMesh = texturesLocation + "/" + meshes[buildingIndex];
        
        float unitConversion;
        vec3 manualOffset = vec3(0.0f);
        switch (buildingIndex) {
            case 0:
                unitConversion = 0.0002f;
//                manualOffset = vec3(0.5f, 0.0f, 10.0f);
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
        
        _mesh = new Mesh(buildingMesh, unitConversion);
        addChild(_mesh);
    }
    
    // Set height
    {
        BoundingBox bb = _mesh->getBoundingBox();
        _height = bb.getSize().y;
        
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

void Building::stretchToFill(vec3 size) {
    _mesh->stretchToFill(size);
}