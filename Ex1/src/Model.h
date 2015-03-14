//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

using namespace glm;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#import <vector>

class Model {
    
    // World transformation
    mat4 _world;
    
    // Terrain vertices
    std::vector<vec3> _terrain;
	
    GLuint _vao, _vbo, _ibo;

    // Attribute handle:
    GLint _posAttrib;
	
    // Uniform handle:
    GLint _fillColorUV, _gpuWVP;
    
    size_t _nVertices;
    
    // View port frame:
    float _width, _height, _offsetX, _offsetY;

 public:
    Model();
    virtual ~Model();
    void init();
    void draw();
    void resize(int width, int height);
    
private:
    void initGrid(int width, int length, float elementSize);
    void pushGridVertices();
};

#endif /* defined(__ex0__Model__) */
