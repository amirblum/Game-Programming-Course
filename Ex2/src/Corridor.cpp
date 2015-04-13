//
//  Corridor.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Corridor.h"
#include "Camera.h"

#include <iostream>
//#include "stb_image.h"
#include "bimage.h"

static const std::string WALL_TEXTURE = "/Users/amirblum/Development/Game-Programming-Course/Ex2/assets/brickwork-texture.bmp";
static const std::string WALL_BUMP = "/Users/amirblum/Development/Game-Programming-Course/Ex2/assets/brickwork-bump-map.bmp";

/**
 * Corridor constructor
 */
Corridor::Corridor(vec3 positionVec, vec3 scaleVec) :
Renderable("corridor", "CorridorShader.vert", "CorridorShader.frag", positionVec, scaleVec),
_width(scaleVec.x),
_height(scaleVec.y),
_length(scaleVec.z),
_offset(0)
{
    // Initialize corridor
    
    static const GLfloat vertices[] = {
        // Left wall
        -0.5f, -0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f,
        
        -0.5f, -0.5f, -0.5f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f,
        
        // Right wall
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f
    };


    // Push VBO
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    static const GLubyte indices[] = {
        0, 1, 2,
        1, 3, 2,
        1, 5, 3,
        3, 7, 5,
        5, 4, 7,
        4, 6, 7,
        4, 0, 6,
        0, 2, 6
    };
    
    _nElementIndices = sizeof(indices) / sizeof(GLubyte);
    
    // Push IBO
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    // Create corridor texture
    {
        // Load wall image
//        int wallImageWidth, wallImageHeight, wallImageChannels;
//        unsigned char *wallImageData = stbi_load(WALL_TEXTURE.c_str(), &wallImageWidth, &wallImageHeight, &wallImageChannels, 0);
        BImage wallImage(WALL_TEXTURE.c_str());
        _texturePercent = wallImage.width() / _length / 100.0f;
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glActiveTexture(GL_TEXTURE0);
        
        // Cubemap texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, _tex);
        
        // Give the image to OpenGL (one for each face)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
                     GL_RGBA,
                     wallImage.width(), wallImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     wallImage.getImageData());
        
        // Format cube map texture
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // Load the bump map
        BImage bumpMap(WALL_BUMP.c_str());
        
        glActiveTexture(GL_TEXTURE1);
        
        glBindTexture(GL_TEXTURE_2D, _bump); // Bind texture before setting its properties
        
        // Give the image to OpenGL (one for each face)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
                     GL_RGBA,
                     bumpMap.width(), bumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     bumpMap.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
                     GL_RGBA,
                     bumpMap.width(), bumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     bumpMap.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
                     GL_RGBA,
                     bumpMap.width(), bumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     bumpMap.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
                     GL_RGBA,
                     bumpMap.width(), bumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     bumpMap.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
                     GL_RGBA,
                     bumpMap.width(), bumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     bumpMap.getImageData());
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
                     GL_RGBA,
                     bumpMap.width(), bumpMap.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     bumpMap.getImageData());
        
        // Format cube map texture
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    // Set custom uniform variables
    {
        _offsetUniform = glGetUniformLocation(_shaderProgram, "offset");
        _texturePercentUniform = glGetUniformLocation(_shaderProgram, "texturePercent");
        
        _lightPosUniform = glGetUniformLocation(_shaderProgram, "lightPos");
        _lightDirUniform = glGetUniformLocation(_shaderProgram, "lightDir");
        
        _lightIntensityUniform = glGetUniformLocation(_shaderProgram, "lightIntensity");
        _lightCutoffUniform = glGetUniformLocation(_shaderProgram, "lightCutoff");
        _lightDarkenStartUniform = glGetUniformLocation(_shaderProgram, "lightDarkenStart");
        _lightDarkenEndUniform = glGetUniformLocation(_shaderProgram, "lightDarkenEnd");
    }
}

void Corridor::customBindings()
{
    // Custom uniform variables
    glUniform1f(_offsetUniform, _offset);
    glUniform1f(_texturePercentUniform, _texturePercent);
    
    glUniform3fv(_lightPosUniform, 1, value_ptr(_lightPos));
    glUniform3fv(_lightDirUniform, 1, value_ptr(_lightDir));
    
    glUniform1f(_lightIntensityUniform, _lightIntensity);
    glUniform1f(_lightCutoffUniform, _lightCutoff);
    glUniform1f(_lightDarkenStartUniform, _length / 4);
    glUniform1f(_lightDarkenEndUniform, _length);
    
//    std::cout << "Light direction: x- " << _lightDir.x << " y- " << _lightDir.y << std::endl;
    
    // Cubemap texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _tex);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _bump);
}

/**
 * Deconstructor
 */
Corridor::~Corridor()
{
}

/**
 * Get width
 */
float Corridor::getWidth()
{
    return _width;
}

/**
 * Get height
 */
float Corridor::getHeight()
{
    return _height;
}

/**
 * Get length
 */
float Corridor::getLength()
{
    return _length;
}

/**
 * Get offset in corridor
 */
float Corridor::getOffset()
{
    return _offset;
}

/**
 * Sets the light position
 */
void Corridor::setLightPos(vec3 lightPos)
{
    _lightPos = lightPos;
}

/**
 * Sets the light direction
 */
void Corridor::setLightDir(vec3 lightDir)
{
    _lightDir = lightDir;
}

/**
 * Sets the light cutoff angle
 */
void Corridor::setLightCutoff(float lightCutoff)
{
    _lightCutoff = lightCutoff;
}

/**
 * Sets the light intensity
 */
void Corridor::setLightIntensity(float lightIntensity)
{
    _lightIntensity = lightIntensity;
}

/**
 * Set the offset. Make sure it always remains between 0 and 1
 */
void Corridor::increaseOffset(float increment)
{
    _offset += increment;
    // Return to (-1,1)
    _offset = _offset - (int)_offset;
}