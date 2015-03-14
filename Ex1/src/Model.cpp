//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"

#define SHADERS_DIR "shaders/"

#define GRID_WIDTH (50)
#define GRID_LENGTH (50)
#define GRID_ELEMENT_SIZE (1.0f)

Model::Model() :
    _vao(0), _vbo(0), _ibo(0), _world(1.0f)
{
}

Model::~Model()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
} 

void Model::init()
{
    programManager::sharedInstance()
	.createProgram("default",
                       SHADERS_DIR "SimpleShader.vert",
                       SHADERS_DIR "SimpleShader.frag");

    GLuint program = programManager::sharedInstance().programWithID("default");
		
    // Obtain uniform variable handles:
    _fillColorUV  = glGetUniformLocation(program, "fillColor");
    _gpuWVP  = glGetUniformLocation(program, "wvp");

    // Initialize vertices buffer and transfer it to OpenGL
    {
        // Create and bind the object's Vertex Array Object:
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
		
        // Create and vertex data and index data Vertex Buffer Objects:
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ibo);
        
        // Init the grid
        initGrid(GRID_WIDTH, GRID_LENGTH, GRID_ELEMENT_SIZE);
        
        // Tell OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
        // Obtain attribute handles:
        _posAttrib = glGetAttribLocation(program, "position");
        glEnableVertexAttribArray(_posAttrib);
        glVertexAttribPointer(_posAttrib, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);
		
        // Unbind vertex array:
        glBindVertexArray(0);
    }
}

void Model::initGrid(int width, int length, float elementSize)
{
    // Populate _terrain vector
    for (int row = 0; row < length; ++row) {
        for (int col = 0; col < width; ++col) {
            // Create grid point
            vec3 vertex;
            vertex.x = col * elementSize;
            vertex.z = row * elementSize;
            vertex.y = 0.0f;
            
            // Add to terrain
            _terrain.push_back(vertex);
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
                std::cout << i << ",";
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
                
                std::cout  << std::endl;
            }
        }
    }
    
    // Push initial (and final) indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}

void Model::pushGridVertices()
{
    // For now, convert to float array for every push.
    // TODO: Save as float array maybe?
    float vertices[_terrain.size() * 4];
    for (int i = 0; i < _terrain.size(); ++i) {
        int index = i * 4;
        vertices[index + 0] = _terrain[i].x;
        vertices[index + 1] = _terrain[i].y;
        vertices[index + 2] = _terrain[i].z;
        vertices[index + 3] = 1.0f;
    }
    
    _nVertices = sizeof(vertices) / sizeof(float);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Model::draw()
{
    // Set the program to be used in subsequent lines:
    glUseProgram(programManager::sharedInstance().programWithID("default"));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Also try using GL_FILL and GL_POINT
  
    mat4 wvp = Camera::Instance().getViewProjection() * _world;
    
    // Pass the uniform variables
    {
        // wvp matrix
        glUniformMatrix4fv (_gpuWVP, 1, GL_FALSE, value_ptr(wvp));
        
        // Model color
        glUniform4f(_fillColorUV, 0.3f, 0.5f, 0.3f, 1.0);
    }

    // Draw using the state stored in the Vertex Array object:
    {
        glBindVertexArray(_vao);
        
        glDrawElements(GL_TRIANGLES, _nVertices, GL_UNSIGNED_SHORT, (GLvoid*)0);

        // Unbind the Vertex Array object
        glBindVertexArray(0);
    }
	
    // Cleanup, not strictly necessary
    glUseProgram(0);
}

void Model::resize(int width, int height)
{
    _width = width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}
