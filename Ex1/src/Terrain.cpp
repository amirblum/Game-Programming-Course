//
//  Terrain.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Terrain.h"
#include "Camera.h"

/**
 * Terrain constructor
 */
Terrain::Terrain(int gridWidth, int gridLength) : _gridWidth(gridWidth), _gridLength(gridLength)
{
    initGrid(gridWidth, gridLength);
}

/**
 * Deconstructor
 */
Terrain::~Terrain()
{
}

/**
 * Initialize the grid using a width and a length
 */
void Terrain::initGrid(int width, int length)
{
    // For now grid always starts with bottom left corner at 0,0,0.
    // This can be changed in the future (though world matrix should be used to position)
    _leftBound = 0;
    _rightBound = width * GRID_ELEMENT_SIZE - 1;
    _frontBound = 0;
    _backBound = length * GRID_ELEMENT_SIZE - 1;
    
    // Populate _grid vector
    for (int row = 0; row < length; ++row) {
        for (int col = 0; col < width; ++col) {
            // Create grid point
            vec3 vertex;
            vertex.x = col * GRID_ELEMENT_SIZE;
            vertex.z = row * GRID_ELEMENT_SIZE;
            vertex.y = 0.0f;
            
            // Add to terrain
            _grid.push_back(vertex);
        }
    }
    
    // Push initial VBO
    pushGridVertices();
    
    // We only need to send the indices once, so create them and send them here
    std::vector<unsigned short> indices;
    for (unsigned short i = 0; i < width * length; ++i) {
        // Determine if we're in the correct row
        int placementInTwoRows = (i + 1) % (width * 2);
        bool oddRow = placementInTwoRows <= width;
        
        // Only use the even vertices
        bool evenVertex = i % 2 == 0;
        
        // For each of these "center" vertices, push the eight surrounding
        // polygons as indices
        if (oddRow && evenVertex)
        {
            // The vertices surrounding this one
            int surroundingVertices[] = {
                i - width - 1,
                i - width,
                i - width + 1,
                i + 1,
                i + width + 1,
                i + width,
                i + width - 1,
                i - 1
            };
            
            // Set helper booleans (for triangles we want to skip
            bool firstRow = i < width;
            bool lastRow = i > width * length - 1 - width;
            bool firstColumn = i % width == 0;
            bool lastColumn = i % width == width - 1;
            
            // Add the ppolygon indices
            for (int j = 0; j < 8; ++j) {
                if (firstRow && (j < 3 || j == 7)) continue;
                if (lastRow && (j >= 3 && j != 7)) continue;
                if (firstColumn && (j > 4 || j == 0)) continue;
                if (lastColumn && (j <= 4 && j != 0)) continue;
                
                // Create polygon originating at i and using the next two surrounding vertices
                indices.push_back(i);
                indices.push_back((unsigned short)surroundingVertices[j]);
                // Loop back when necessary
                if (j == 7)
                {
                    indices.push_back((unsigned short)surroundingVertices[0]);
                }
                else
                {
                    indices.push_back((unsigned short)surroundingVertices[j + 1]);
                }
            }
        }
    }
    
    _nElements = indices.size();
    
    // Push initial (and final) indices
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);

}

/**
 * Convert terrain vector to vertex array and push to OpenGL
 */
void Terrain::pushGridVertices()
{
    // For now, convert to float array for every push.
    // TODO: Save as float array maybe?
    float vertices[_grid.size() * 4];
    for (int i = 0; i < _grid.size(); ++i) {
        int index = i * 4;
        vertices[index + 0] = _grid[i].x;
        vertices[index + 1] = _grid[i].y;
        vertices[index + 2] = _grid[i].z;
        vertices[index + 3] = 1.0f;
    }
    
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

/**
 * Get grid width
 */
int Terrain::getGridWidth()
{
    return _gridWidth;
}

/**
 * Get grid length
 */
int Terrain::getGridLength()
{
    return _gridLength;
}

/**
 * Get left bound of terrain
 */
float Terrain::getLeftBound()
{
    return _leftBound;
}

/**
 * Get right bound of terrain
 */
float Terrain::getRightBound()
{
    return _rightBound;
}

/**
 * Get front bound of terrain
 */
float Terrain::getFrontBound()
{
    return _frontBound;
}

/**
 * Get back bound of terrain
 */
float Terrain::getBackBound()
{
    return _backBound;
}

/**
 * Return the number of a vertex based on x,y coordinates (of the grids 2D matrix)
 */
int Terrain::getVertexFromCoords(int x, int y)
{
    return _gridWidth * y + x;
}

/**
 * Return the height of the numbered vertex
 * -1 if out of bounds
 */
float Terrain::getVertexHeight(int vertex)
{
    if (vertex < 0 || vertex >= _grid.size()) return -1.f;
    return _grid[vertex].y;
}

/**
 * Set the height of a numbered vertex
 */
void Terrain::setVertexHeight(int vertex, float newHeight)
{
    if (vertex < 0 || vertex >= _grid.size()) return;
    _grid[vertex].y = newHeight;
}

/**
 * Get height of vertex based on coordinates
 * Returns -1 if vertex doesn't exist
 */
float Terrain::getVertexHeight(int x, int y)
{
    int vertex = getVertexFromCoords(x, y);
    if (vertex == -1) {
        return -1.0f;
    }
    return getVertexHeight(vertex);
}

/**
 * Set height of vertex based on coordinates
 */
void Terrain::setVertexHeight(int x, int y, float newHeight)
{
    int vertex = getVertexFromCoords(x, y);
    if (vertex == -1) {
        return;
    }
    setVertexHeight(vertex, newHeight);
}

/**
 * Gets interpolated height of four surrounding vertices of a point in the world
 */
float Terrain::interpolatedHeight(float x, float z)
{
    // Get heights of the four surrounding vertices
    float bottomLeftHeight = getVertexHeight((int)x, (int)z);
    float bottomRightHeight = getVertexHeight((int)(x+1), (int)z);
    float topLeftHeight = getVertexHeight((int)x, (int)(z+1));
    float topRightHeight = getVertexHeight((int)(x+1), (int)(z+1));
    
    float xSquareOffset = x - (_leftBound + (int)x * GRID_ELEMENT_SIZE);
    float zSquareOffset = z - (_frontBound + (int)z * GRID_ELEMENT_SIZE);
    
    // Interpolation weight
    float xWeight = xSquareOffset / GRID_ELEMENT_SIZE;
    float zWeight = zSquareOffset / GRID_ELEMENT_SIZE;
    
    // Interpolate bottom and top
    float bottomInterpolatedHeight = mix(bottomLeftHeight, bottomRightHeight, xWeight);
    float topInterpolatedHeight = mix(topLeftHeight, topRightHeight, xWeight);
    
    // Then interpolate the to previous
    float interpolatedHeight = mix(bottomInterpolatedHeight, topInterpolatedHeight, zWeight);
    
    return interpolatedHeight;
}