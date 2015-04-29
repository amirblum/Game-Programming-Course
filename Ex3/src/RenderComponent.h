//
//  RenderComponent.h
//  CGP-Ex3
//
//  Component for groups of vertices that can be rendered by OpenGL
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__RenderComponent__
#define __CGP_Ex3__RenderComponent__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <unordered_map>

#include "SceneNode.h"

struct TextureComponent {
    GLuint texture;
    int textureNumber;
    GLenum type;
    GLint samplerUniform;
};

enum UniformType {
    UNIFORM_NONE,
    UNIFORM_FLOAT,
    UNIFORM_INT,
    UNIFORM_BOOLEAN,
    UNIFORM_VEC3,
    UNIFORM_VEC4,
    UNIFORM_MAT3,
    UNIFORM_MAT4
};

struct UniformVariable {
    GLint handle;
    virtual UniformType getType() {return UNIFORM_NONE;}
};

template <class T, UniformType type>
struct UniformVariableDerived : public UniformVariable {
    T value;
    virtual UniformType getType() {return type;}
};

typedef UniformVariableDerived<float, UNIFORM_FLOAT> UniformVariableFloat;
typedef UniformVariableDerived<int, UNIFORM_INT> UniformVariableInt;
typedef UniformVariableDerived<bool, UNIFORM_BOOLEAN> UniformVariableBool;
typedef UniformVariableDerived<vec3, UNIFORM_VEC3> UniformVariableVec3;
typedef UniformVariableDerived<vec4, UNIFORM_VEC4> UniformVariableVec4;
typedef UniformVariableDerived<mat3, UNIFORM_MAT3> UniformVariableMat3;
typedef UniformVariableDerived<mat4, UNIFORM_MAT4> UniformVariableMat4;


class RenderComponent {
private:
    // Buffer handlers
    GLuint _vao, _vbo, _ibo, _tex, _bump;
    
    // Attribute handle:
    GLint _posAttrib;
    
    // Shader program ID
    GLuint _shaderProgram;
    
    // Textures
    unsigned int _textureCount;
    std::vector<TextureComponent*> _textures;
    void sendTexture(TextureComponent *textureComponent);
    
    // Uniform handles:
    GLint _worldUniform, _viewProjectionUniform,
    _textureUniform, _bumpUniform;
    std::unordered_map<std::string, UniformVariable*> _uniforms;
    void sendUniform(UniformVariable *uniform);
    
    // Number of elements
    size_t _nElementIndices;
    
    virtual void customBindings();
    
public:
    RenderComponent(std::string shaderProgram,
               std::string vertexShaderFilename,
               std::string fragmentShaderFilename);
    virtual ~RenderComponent();
    
    void render(mat4 worldView);
    
    void setVBO(std::vector<GLfloat> vertices);
    void setIBO(std::vector<GLubyte> indices);
    void addTexture(std::string textureFile, GLenum textureType);
    template <class T, UniformType type>
    void setUniform(std::string name, T value);
};

#endif /* defined(__CGP_Ex3__RenderComponent__) */
