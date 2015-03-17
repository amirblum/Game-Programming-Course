//
//  ParticleDeposition.h
//  CGP-Ex1
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__ParticleDeposition__
#define __CGP_Ex1__ParticleDeposition__

#include "TerrainDeformer.h"

class ParticleDeposition : public TerrainDeformer {
    float _displacement;
    int _currentX, _currentY;
    int _iteration, _currentIterations;
    
public:
    ParticleDeposition(Terrain *terrain, float displacement);
    void performDeformationStep();
    void depositParticle(int x, int y);
    
private:
    void beginNewMountain();
};

#endif /* defined(__CGP_Ex1__ParticleDeposition__) */
