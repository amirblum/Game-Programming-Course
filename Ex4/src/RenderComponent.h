//
//  RenderComponent.h
//  CGP-Ex4
//
//  Component for groups of vertices that can be rendered by OpenGL
//
//  Created by Amir Blum on 3/16/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__RenderComponent__
#define __CGP_Ex4__RenderComponent__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <unordered_map>

#include "SceneNode.h"

struct Vertex {
    vec3 position;
    vec2 texcoords;
    vec3 normal;
    
    Vertex() {}
    
    Vertex(const vec3 &position, const vec2 &texcoords, const vec3 &normal) :
    position(position), texcoords(texcoords), normal(normal) {}
};

class TextureComponent {
private:
    GLenum _type;
    GLuint _textureNum;
    
public:
    TextureComponent(GLenum type, GLuint textureNum);
    static std::shared_ptr<TextureComponent> create2DTexture(std::string filename);
    static std::shared_ptr<TextureComponent> createCubeTexture(std::string cubeLF,
                                               std::string cubeFT,
                                               std::string cubeRT,
                                               std::string cubeBK,
                                               std::string cubeUP,
                                               std::string cubeDN);
    virtual ~TextureComponent();
    void bind();
};

enum UniformType {
    UNIFORM_NONE,
    UNIFORM_FLOAT,
    UNIFORM_INT,
    UNIFORM_BOOLEAN,
    UNIFORM_VEC2,
    UNIFORM_VEC3,
    UNIFORM_VEC4,
    UNIFORM_MAT3,
    UNIFORM_MAT4
};

class UniformVariable {
public:
    virtual ~UniformVariable() {}
    
    GLint handle;
    virtual UniformType getType() = 0;
};

template <class T, UniformType type>
class UniformVariableDerived : public UniformVariable {
public:
    virtual ~UniformVariableDerived() {}
    
    T value;
    virtual UniformType getType() {return type;}
};

typedef UniformVariableDerived<float, UNIFORM_FLOAT> UniformVariableFloat;
typedef UniformVariableDerived<int, UNIFORM_INT> UniformVariableInt;
typedef UniformVariableDerived<bool, UNIFORM_BOOLEAN> UniformVariableBool;
typedef UniformVariableDerived<vec2, UNIFORM_VEC2> UniformVariableVec2;
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
    std::vector<std::shared_ptr<TextureComponent>> _textures;
    
    // Uniform handles:
    std::unordered_map<std::string, UniformVariable*> _uniforms;
    void sendUniform(UniformVariable *uniform);
    
    // Number of elements
    unsigned int _numIndices;
    
    // Polygon options
    GLenum _polyFace, _polyMode;
    
public:
    RenderComponent(std::string shaderProgram);
    virtual ~RenderComponent();
    
    void render(mat4 worldView);
    void render(mat4 worldView, int numInstances);
    
    void setPolygonFace(GLenum face);
    void setPolygonDrawMode(GLenum mode);
    
    void setPTNVBO(std::vector<Vertex> vertices);
    void setPositionsVBO(std::vector<GLfloat> vertices);
    void setIBO(std::vector<GLuint> indices);
    
    GLuint createSupportVBO(GLenum type, int size, std::string name, int divisor);
    
    void addTexture(std::shared_ptr<TextureComponent> textureComponent);

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
        } else {
            uniform = dynamic_cast<UniformVariableDerived<T, type>*>(uniformIterator->second);
        }
        
        uniform->value = value;
    }
};

#endif /* defined(__CGP_Ex4__RenderComponent__) */
