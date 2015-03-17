//
//  ParticleDeposition.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "ParticleDeposition.h"
#include <iostream>

ParticleDeposition::ParticleDeposition(Terrain *terrain, float displacement) :
TerrainDeformer(terrain),
_displacement(displacement),
_iteration(0)
{
    beginNewMountain();
}

void ParticleDeposition::beginNewMountain()
{
    // Initialize drop point to random point to begin
    do {
        _currentX = rand() % _terrain->getGridWidth();
        _currentY = rand() % _terrain->getGridLength();
    } while (_terrain->getVertexHeight(_currentX, _currentY) > 0);
    
    // Get random iterations for mountain
    int terrainSize = _terrain->getGridWidth() * _terrain->getGridLength();
    int quarterTerrain = terrainSize / 4;
    int eighthTerrain = quarterTerrain / 2;
    
    _currentIterations = rand() % (quarterTerrain - eighthTerrain) + eighthTerrain;
    _iteration = 0;
    
    std::cout << "Starting deposition with " << _currentIterations << " iterations" << std::endl;
}

void ParticleDeposition::performDeformationStep()
{
    // Check if to move to next mountain
    _iteration++;
    if (_iteration > _currentIterations)
    {
        beginNewMountain();
    }
    
    // Advance in random direction
    int direction = rand() % 4;
    switch (direction) {
        case 0:
            _currentX++;
            if (_currentX == _terrain->getGridWidth())
            {
                _currentX = 0;
            }
            break;
        case 1:
            _currentX--;
            if (_currentX == -1)
            {
                _currentX = _terrain->getGridWidth() - 1;
            }
            break;
        case 2:
            _currentY++;
            if (_currentY == _terrain->getGridLength())
            {
                _currentY = 0;
            }
            break;
        case 3:
            _currentY--;
            if (_currentY == -1)
            {
                _currentY = _terrain->getGridLength() - 1;
            }
            break;
    }
    
    // "Deopsit" particle
    depositParticle(_currentX, _currentY);
    
    // Update vbo
    _terrain->pushGridVertices();
}

void ParticleDeposition::depositParticle(int x, int y)
{
    float myHeight = _terrain->getVertexHeight(x, y);
    std::vector<vec2> lowerNeighbors;
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            // Assign current neighbor
            int neighborX = x - 1 + i;
            int neighborY = y - 1 + i;
            
            // Skip test with self
            if (neighborX == x && neighborY == y) continue;
            
            // Check height against self
            float neighborHeight = _terrain->getVertexHeight(neighborX, neighborY);
            if (neighborHeight < myHeight)
            {
                lowerNeighbors.push_back(vec2(neighborX, neighborY));
            }
        }
    }
    
    if (lowerNeighbors.size() > 0)
    {
        // Choose a random neighbor to roll to
        int randomNeighbor = rand() % lowerNeighbors.size();
        depositParticle(lowerNeighbors[randomNeighbor].x, lowerNeighbors[randomNeighbor].y);
    }
    else
    {
        // Raise this vertex
        _terrain->setVertexHeight(x, y, myHeight + _displacement);
    }
    
}