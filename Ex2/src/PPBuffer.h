//
//  PPBuffer.h
//  cg-projects
//
//  Created by HUJI Computer Games Programming course staff, 2014.
//

#ifndef __ex2__PPBuffer__
#define __ex2__PPBuffer__

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

class PPBuffer {
    GLuint _vao;
    GLuint vbo_fbo_vertices;
    GLuint fbo, fbo_texture, rbo_depth;
    GLuint program_postproc, attribute_v_coord_postproc, uniform_fbo_texture;

    float _width, _height, _offsetX, _offsetY;
    
    // Convolution
    mat3 _convoKernel;
    GLuint _convoKernelUniform;
    
    // Pixel-based PP
    GLuint _uStepUniform, _vStepUniform;

 public:
    PPBuffer(int screen_width, int screen_height);
    virtual ~PPBuffer();
    void setup();
    
    void switchConvolutionKernel(int kernel);
    void setConvolutionKernel(mat3 kernel);
    mat3 getIdentityConvolution();
    mat3 getGaussianBlurConvolution();
    mat3 getSharpenConvolution();
    mat3 getEdgeDetectConvolution();
    mat3 getEmbossConvolution();
    
    void render();
    void resize(int width, int height);
};

#endif /* defined(__ex2_PPBuffer_) */
