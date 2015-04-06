//
//  Renderable.h
//  CGP-Ex2
//
//  Base class for groups of vertices that can be rendered by OpenGL
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex2__Renderable__
#define __CGP_Ex2__Renderable__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
using namespace glm;

class Renderable {
protected:
    // World transformation
    mat4 _world;
    
    // Buffer handlers
    GLuint _vao, _vbo, _ibo;
    
    // Attribute handle:
    GLint _posAttrib;
    
    // Uniform handle:
    GLint _fillColorUV, _gpuWVP;
    
    // Number of elements
    size_t _nElementIndices;
    
public:
    Renderable();
    virtual ~Renderable();
    void draw();
};

#endif /* defined(__CGP_Ex2__Renderable__) */
