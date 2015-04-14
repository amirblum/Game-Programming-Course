//
//  Monster.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 4/14/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Monster.h"
#include "bimage.h"

#ifdef __APPLE__
#include <irrklang.h>
using namespace irrklang;
#endif

#include "glm/gtc/matrix_transform.hpp"

#define MONSTER_SPEED (5.0f)

static const std::string MONSTER_TEXTURE = "assets/monster.bmp";

Monster::Monster(float width, float height, vec3 startPosition, float lightDarkenStart) :
Renderable("monster", "MonsterShader.vert", "MonsterShader.frag",
           startPosition, vec3(width, height, 0.0f)),
_lightDarkenStart(lightDarkenStart)
{
    // Initialize monster vertices (just s flat quad)
    {
        static const GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f,
        };
        
        // Push VBO
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    
    // Indices
    {
        static const GLubyte indices[] = {
            0, 1, 2,
            1, 2, 3
        };
        
        _nElementIndices = sizeof(indices) / sizeof(GLubyte);
        
        // Push IBO
        glBindVertexArray(_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    
    // Create texture
    {
        BImage monsterImage;
        if (!monsterImage.readImage(MONSTER_TEXTURE.c_str())) {
            std::cout << "Failed to read monster texture" << std::endl;
            exit(1);
        }
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glActiveTexture(GL_TEXTURE0);
        
        // Cubemap texture
        glBindTexture(GL_TEXTURE_2D, _tex);
        
        // Give the image to OpenGL (one for each face)
        glTexImage2D(GL_TEXTURE_2D, 0,
                     GL_RGBA,
                     monsterImage.width(), monsterImage.height(),
                     0,
                     GL_RGB, GL_UNSIGNED_BYTE,
                     monsterImage.getImageData());
        
        // Setting texture properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    // Custom uniform variables
    {
        _lightDarkenStartUniform = glGetUniformLocation(_shaderProgram, "lightDarkenStart");
    }
}

void Monster::customBindings()
{
    glUniform1f(_lightDarkenStartUniform, _lightDarkenStart);
    
    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex);
}

bool soundPlayed = false;
void Monster::moveForward(float dt)
{
    mat4 advanceTranslate = translate(mat4(1.0f), vec3(0.0f, 0.0f, 1.0f * MONSTER_SPEED * dt));
    _world = advanceTranslate * _world;
    
#ifdef __APPLE__
    if (!soundPlayed) {
        vec4 startPosition(0.0f, 0.0f, -120.0f, 1.0f);
        vec4 currentPosition = _world * startPosition;
        if (currentPosition.z > -30.0f) {
            // start the sound engine with default parameters
            ISoundEngine* engine = createIrrKlangDevice();
            
            if (!engine)
            {
                printf("Could not startup engine\n");
                return; // error starting up the engine
            }
            
            // To play a sound, we only to call play2D(). The second parameter
            // tells the engine to play it looped.
            
            // play some sound stream, looped
            engine->play2D("assets/scream.ogg");
            soundPlayed = true;
        }
    }
#endif
}