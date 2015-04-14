//
//  Corridor.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex2__Corridor__
#define __ex2__Corridor__

#include "Renderable.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Corridor : public Renderable {
private:
    // Info
    float _width, _height, _length, _offset, _texturePercent;
    
    // Light info
    vec3 _lightPos, _lightDir;
    float _lightIntensity, _lightCutoff;
    float _darknessComponent;
    
    // Glow
    bool _drawGlow;
    
    // Custom uniform variables
    GLuint _offsetUniform, _texturePercentUniform;
    GLuint _lightPosUniform, _lightDirUniform;
    GLuint _lightCutoffUniform, _lightIntensityUniform,
           _lightDarkenStartUniform, _lightDarkenEndUniform,
           _darknessComponentUniform,
           _drawGlowUniform;
    
// Overriding
public:
    void customBindings();

// Personal
public:
    Corridor(vec3 positionVec, vec3 scaleVec);
    virtual ~Corridor();
    
    void drawGlow();
    
    // Getters
    float getWidth();
    float getHeight();
    float getLength();
    float getOffset();
    
    // Setters
    void setLightPos(vec3 lightPos);
    void setLightDir(vec3 lightDir);
    void setLightCutoff(float lightCutoff);
    void setLightIntensity(float lightIntensity);
    void changeDarkness(bool increase);
    void increaseOffset(float increment);
};

#endif /* defined(__ex2__Corridor__) */
