//
//  Road.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Road.h"
#include "RandUtils.h"

Road::Road(std::string filename,
           float unitConversion,
           vec3 position, quat rotation) :
SceneNode(position)
{
    // Mesh
    {
        _mesh = new Mesh(filename, unitConversion, vec3(0.0f), rotation);
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

Road::~Road()
{
    
}

void Road::stretchToFill(vec3 size) {
    _mesh->stretchToFill(size);
}