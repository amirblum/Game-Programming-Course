//
//  CityBlock.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CityBlock.h"
#include "RandUtils.h"

static const std::string buildingMeshes[] = {
    "assets/buildings/building1/building1.obj",
    "assets/buildings/building2/building2.obj",
    "assets/buildings/building3/building3.obj",
    "assets/buildings/building4/building4.obj"
};

static const std::string roadMesh = "assets/road/road1.obj";
static const std::string intersectionMesh = "assets/intersection/road3.obj";

CityBlock::CityBlock(vec3 position, float buildingWidth, float roadWidth) :
SceneNode(position)
{
    // Initialize random building
    {
        int buildingIndex = randutils::randomRange(0, 4);
//        int buildingIndex = 3;
        
        std::string buildingMeshString = buildingMeshes[buildingIndex];
        
        float unitConversion;
        vec3 bbSizeDiff = vec3(0.0f);
        switch (buildingIndex) {
            case 0:
                unitConversion = 0.0002f;
                bbSizeDiff = vec3(-0.65f, 0.0f, -0.65f);
                break;
            case 1:
                unitConversion = 0.01f;
                bbSizeDiff = vec3(-0.75f, 0.0f, -0.75f);
                break;
            case 2:
                unitConversion = 0.005f;
                bbSizeDiff = vec3(-0.85f, 0.0f, -0.3f);
                break;
            case 3:
                unitConversion = 0.005f;
                bbSizeDiff = vec3(-0.5f, 0.0f, -0.95f);
                break;
            default:
                unitConversion = 1.0f;
                break;
        }
        
        Mesh *buildingMesh = new Mesh(buildingMeshString, unitConversion);
        
        vec3 bbSize = buildingMesh->getBoundingBox().getSize();
        BoundingBox customBB(bbSize + bbSizeDiff);
        buildingMesh->setCustomBoundingBox(BoundingBox(customBB));
        buildingMesh->stretchToFill(vec3(buildingWidth, -1.0f, buildingWidth));
        
        
        vec3 buildingPosition = vec3(0.0f, customBB.getSize().y / 2.0f, 0.0f);
        buildingMesh->setPosition(buildingPosition);
        
        addChild(buildingMesh);
    }
    
    // Set roads
    {
        float roadOffset = (buildingWidth + roadWidth) / 2.0f;
        
        // Upper road
        Mesh *upperRoad = new Mesh(roadMesh, 0.01f, vec3(0.0f), quat(vec3(0.0f, pi<float>() / 2.0f, 0.0f)));
        upperRoad->stretchToFill(vec3(roadWidth, -1.0f, buildingWidth));
        
        vec3 upperRoadPos = vec3(0.0f, upperRoad->getBoundingBox().getSize().y / 2.0f, roadOffset);
        upperRoad->setPosition(upperRoadPos);
        
        addChild(upperRoad);
        
        // Side road
        Mesh *sideRoad = new Mesh(roadMesh, 0.01f, vec3(0.0f), quat(vec3(0.0f)));
        sideRoad->stretchToFill(vec3(roadWidth, -1.0f, buildingWidth));
        
        vec3 sideRoadPos = vec3(roadOffset, sideRoad->getBoundingBox().getSize().y / 2.0f, 0.0f);
        sideRoad->setPosition(sideRoadPos);
        
        addChild(sideRoad);
        
        // Intersection
        Mesh *intersection = new Mesh(intersectionMesh, 0.01f, vec3(0.0f), quat(vec3(0.0f)));
        intersection->stretchToFill(vec3(roadWidth, -1.0f, roadWidth));
        
        vec3 intersectionPos = vec3(roadOffset, intersection->getBoundingBox().getSize().y / 2.0f, roadOffset);
        intersection->setPosition(intersectionPos);
        
        addChild(intersection);
    }
}

CityBlock::~CityBlock()
{
    
}