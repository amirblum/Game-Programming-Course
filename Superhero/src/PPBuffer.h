//
//  PPBuffer.h
//  cg-projects
//
//  Created by HUJI Computer Games Programming course staff, 2014.
//

#ifndef __CGP_Superhero__PPBuffer__
#define __CGP_Superhero__PPBuffer__

#define GLM_FORCE_RADIANS
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
    GLuint fbo, fbo_color_texture, fbo_depth_texture;
    GLuint program_postproc, attribute_v_coord_postproc;
    GLuint uniform_fbo_color_texture, uniform_fbo_depth_texture, uniform_inverseViewProj, uniform_prevViewProj;
    
    mat4 _inverseViewProj, _prevViewProj;

 public:
    PPBuffer(int screen_width, int screen_height);
    virtual ~PPBuffer();
    void setup();
    GLuint getTexture();
    
    void render(bool toScreen);
    void resize(int width, int height);
};

#endif /* defined(__CGP_Superhero_PPBuffer_) */
