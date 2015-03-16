//
//  Terrain.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Terrain__
#define __ex0__Terrain__

#include "Renderable.h"

//using namespace glm;

#import <vector>

class Terrain : public Renderable {
    // Terrain vertices
    std::vector<vec3> _grid;
    
    // Info
    int _gridWidth, _gridLength;

 public:
    Terrain(int gridWidth, int gridLength);
    virtual ~Terrain();
    
    int getGridWidth();
    int getGridLength();
    
private:
    void initGrid(int width, int length);
    void pushGridVertices();
};

#endif /* defined(__ex0__Terrain__) */
