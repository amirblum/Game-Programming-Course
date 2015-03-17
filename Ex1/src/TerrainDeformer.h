//
//  TerrainDeformer.h
//  CGP-Ex1
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__TerrainDeformer__
#define __CGP_Ex1__TerrainDeformer__

#include "Terrain.h"

class TerrainDeformer {
protected:
    Terrain *_terrain;
    
public:
    TerrainDeformer(Terrain *terrain);
    virtual void performDeformationStep() = 0;
};

#endif /* defined(__CGP_Ex1__TerrainDeformer__) */
