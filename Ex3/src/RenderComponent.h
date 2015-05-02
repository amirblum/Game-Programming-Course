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
    GLuint _vao, _vbo, _ibo;
    std::vector<GLuint> _supportVBOs;
    
    // Shader program ID
    GLuint _shaderProgram;
    
    // Textures
    unsigned int _textureCount;
    std::vector<TextureComponent*> _textures;
    void sendTexture(TextureComponent *textureComponent);
    
    // Uniform handles:
    std::unordered_map<std::string, UniformVariable*> _uniforms;
    void sendUniform(UniformVariable *uniform);
    
    // Number of elements
    size_t _nElementIndices;
    
public:
    RenderComponent(std::string shaderProgram);
    virtual ~RenderComponent();
    
    void render(mat4 worldView);
    void render(mat4 worldView, int numInstances);
    
    void setVBO(std::vector<vec4> vertices);
    void setVBO(std::vector<GLfloat> vertices);
    void setIBO(std::vector<GLubyte> indices);
    
    GLuint createSupportVBO(GLenum type, int size, std::string name, int divisor);
    void addTexture(std::string textureFile, GLenum textureType);

    /**
     * Damnits.. I hate including implementations in header files but it's a must
     * when using cool template functions apparently...
     */
    template <class T>
    void updateSupportVBO(GLuint vbo, std::vector<T> attributes)
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(T), &attributes[0], GL_DYNAMIC_DRAW);
    }
    
    template <class T, UniformType type>
    void setUniform(std::string name, T value)
    {
        UniformVariableDerived<T, type> *uniform;
        auto uniformIterator = _uniforms.find(name);
        if (uniformIterator == _uniforms.end()) {
            // Create the uniform the first time
            uniform = new UniformVariableDerived<T, type>();
            uniform->handle = glGetUniformLocation(_shaderProgram, name.c_str());
            _uniforms[name] = uniform;
            std::cout << "Created handle " << uniform->handle << " for variable " << name << std::endl;
        } else {
            uniform = dynamic_cast<UniformVariableDerived<T, type>*>(uniformIterator->second);
        }
        
        uniform->value = value;
    }
};

#endif /* defined(__CGP_Ex3__RenderComponent__) */
