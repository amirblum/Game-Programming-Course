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
    
    // Vertex helper
    int getVertexFromCoords(int x, int y);
    
    // Height according to vertex
    float getVertexHeight(int vertex);
    void setVertexHeight(int vertex, float newHeight);
    
    // Height according to coordinates
    float getVertexHeight(int x, int y);
    void setVertexHeight(int x, int y, float newHeight);
    
    void pushGridVertices();
    
private:
    void initGrid(int width, int length);
};

#endif /* defined(__ex0__Terrain__) */
