//
//  ParticleDeposition.h
//  CGP-Ex1
//
//  An implementation of TerrainDeformer that uses particle deposition
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__ParticleDeposition__
#define __CGP_Ex1__ParticleDeposition__

#include "TerrainDeformer.h"

class ParticleDeposition : public TerrainDeformer {
    // Private variables
    float _displacement;
    int _currentX, _currentY;
    int _iteration, _currentIterations;
    bool _valleyMode;
    
public:
    ParticleDeposition(Terrain *terrain, float displacement);
    // Overriden function
    void performDeformationStep();

private:
    // Helper functions
    void beginNewChunk();
    void depositParticle(int x, int y);
};

#endif /* defined(__CGP_Ex1__ParticleDeposition__) */
