//
//  MidPointDisplacement.h
//  CGP-Ex1
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__MidPointDisplacement__
#define __CGP_Ex1__MidPointDisplacement__

#include "TerrainDeformer.h"
#include "Terrain.h"

class MidPointDisplacement : public TerrainDeformer {
private:
    // What step are we on?
    int _deformationStep;
    float _maximumDisplacement;
    float _roughness;
    
    // Current vertices we are working with
    int _A, _B, _C, _D, _E, _F, _G, _H, _I;
    
    // Helper function
    float randomDisplacement();
    
public:
    MidPointDisplacement(Terrain *terrain, float startingDisplacement, float roughness);
    void performDeformationStep();
};

#endif /* defined(__CGP_Ex1__MidPointDisplacement__) */
