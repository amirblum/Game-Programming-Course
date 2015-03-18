//
//  Terrain.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Terrain__
#define __ex0__Terrain__

#include "Renderable.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Terrain : public Renderable {
    // Terrain vertices
    std::vector<vec3> _grid;
    
    // Info
    int _gridWidth, _gridLength;
    float _leftBound, _rightBound, _frontBound, _backBound;

 public:
    Terrain(int gridWidth, int gridLength);
    virtual ~Terrain();
    
    // Getters
    int getGridWidth();
    int getGridLength();
    float getLeftBound();
    float getRightBound();
    float getFrontBound();
    float getBackBound();
    
    // Vertex helper
    int getVertexFromCoords(int x, int y);
    
    // Height according to vertex
    float getVertexHeight(int vertex);
    void setVertexHeight(int vertex, float newHeight);
    
    // Height according to coordinates
    float getVertexHeight(int x, int y);
    void setVertexHeight(int x, int y, float newHeight);
    
    // Linearly-interpolated hieght
    float interpolatedHeight(float x, float z);
    
    // Updates OpenGL to the changes
    void pushGridVertices();
    
private:
    // Initialize VBO and IBO
    void initGrid(int width, int length);
};

#endif /* defined(__ex0__Terrain__) */
