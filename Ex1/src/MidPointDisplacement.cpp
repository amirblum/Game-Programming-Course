//
//  MidPointDisplacement.cpp
//  CGP-Ex1
//
//  Implementation of the MidPoint Displacement algorithm.
//  With help from the fine folks at lighthouse3d.com.
//  http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "MidPointDisplacement.h"

#include <iostream>

MidPointDisplacement::MidPointDisplacement(Terrain *terrain, float startingDisplacement, float roughness) :
TerrainDeformer(terrain),
_deformationStep(0),
_maximumDisplacement(startingDisplacement),
_roughness(roughness)
{
    if (terrain->getGridWidth() != terrain->getGridLength()) {
        std::cout << "Warning! Attempting to use MidPoint Displacement on a rectangle terrain!" << std::endl;
    }
    
    int lastRow = terrain->getGridLength() - 1;
    int lastCol = terrain->getGridWidth() - 1;

    // Initialize vertices for first step
    _A = getVertexFromCoords(0, 0);
    _B = getVertexFromCoords(0, lastCol);
    _C = getVertexFromCoords(lastRow, 0);
    _D = getVertexFromCoords(lastRow, lastCol);
    _E = getVertexFromCoords(lastRow / 2, lastCol / 2);
    _F = getVertexFromCoords(lastRow / 2, 0);
    _G = getVertexFromCoords(0, lastCol / 2);
    _H = getVertexFromCoords(lastRow / 2, lastCol);
    _I = getVertexFromCoords(lastRow, lastCol / 2);
}

int MidPointDisplacement::getVertexFromCoords(int x, int y)
{
    return _terrain->getGridWidth() * y + x;
}

float MidPointDisplacement::randomDisplacement()
{
    float scale = ((float)rand()) / (float)RAND_MAX;
    return _maximumDisplacement * 2 * scale - _maximumDisplacement;
}

void MidPointDisplacement::performDeformationStep()
{
    float aHeight = _terrain->getVertexHeight(_A);
    float bHeight = _terrain->getVertexHeight(_B);
    float cHeight = _terrain->getVertexHeight(_C);
    float dHeight = _terrain->getVertexHeight(_D);
    
    // Diamond step
    float newEHeight = (aHeight + bHeight + cHeight + dHeight) / 4 + randomDisplacement();
    _terrain->setVertexHeight(_E, newEHeight);
    
    // Square step
    float newFHeight = (aHeight + cHeight + newEHeight) / 3 + randomDisplacement();
    float newGHeight = (aHeight + bHeight + newEHeight) / 3 + randomDisplacement();
    float newHHeight = (bHeight + dHeight + newEHeight) / 3 + randomDisplacement();
    float newIHeight = (cHeight + dHeight + newEHeight) / 3 + randomDisplacement();
    
    _terrain->setVertexHeight(_F, newFHeight);
    _terrain->setVertexHeight(_G, newGHeight);
    _terrain->setVertexHeight(_H, newHHeight);
    _terrain->setVertexHeight(_I, newIHeight);
    
    // Tell terrain to update
    _terrain->pushGridVertices();
    
    // Update max-displacement
    _maximumDisplacement *= pow(2, -_roughness);
    
    _deformationStep++;
}