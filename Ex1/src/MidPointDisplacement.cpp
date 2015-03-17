//
//  MidPointDisplacement.cpp
//  CGP-Ex1
//
//  Implementation of the MidPoint Displacement algorithm.
//  With help from the fine folks at lighthouse3d.com.
//  http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
//
//  ...so this is actually not finished. It only performs the first
//  step right now. It was abandoned for the sake of Particle Deposition
//  which better suited the grid I had created.
//  I will leave this class here for...posterity, or something.
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
    _A = _terrain->getVertexFromCoords(0, 0);
    _B = _terrain->getVertexFromCoords(0, lastCol);
    _C = _terrain->getVertexFromCoords(lastRow, 0);
    _D = _terrain->getVertexFromCoords(lastRow, lastCol);
    _E = _terrain->getVertexFromCoords(lastRow / 2, lastCol / 2);
    _F = _terrain->getVertexFromCoords(lastRow / 2, 0);
    _G = _terrain->getVertexFromCoords(0, lastCol / 2);
    _H = _terrain->getVertexFromCoords(lastRow / 2, lastCol);
    _I = _terrain->getVertexFromCoords(lastRow, lastCol / 2);
}

/**
 * Get a random float between [-d, d]
 */
float MidPointDisplacement::randomDisplacement()
{
    float scale = ((float)rand()) / (float)RAND_MAX;
    return _maximumDisplacement * 2 * scale - _maximumDisplacement;
}

/**
 * Perform the deformation step. Currently this is only the first phase
 */
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
    
    // Update max-displacement
    _maximumDisplacement *= pow(2, -_roughness);
    
    // Tell terrain to update
    _terrain->pushGridVertices();
    
    _deformationStep++;
}