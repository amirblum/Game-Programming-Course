//
//  ParticleDeposition.h
//  CGP-Ex1
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__ParticleDeposition__
#define __CGP_Ex1__ParticleDeposition__

#include <stdio.h>
#include "TerrainDeformer.h"

class ParticleDeposition : public TerrainDeformer {
private:
    float _displacement;
    int _currentX, _currentY;
public:
    ParticleDeposition(Terrain *terrain, float displacement);
    void performDeformationStep();
    void depositParticle(int x, int y);
};

#endif /* defined(__CGP_Ex1__ParticleDeposition__) */
