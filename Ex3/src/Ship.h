//
//  Ship.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex3__Ship__
#define __ex3__Ship__

#include "SceneNode.h"
#include "RenderComponent.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Ship : public SceneNode {
private:
    RenderComponent _renderComponent;
//    // Light info
//    vec3 _lightPos, _lightDir;
//    
//    // Custom uniform variables
//    GLuint _lightPosUniform, _lightDirUniform;
    
//// Overriding
//public:
//    void customBindings();

// Personal
public:
    Ship(vec3 position, vec3 rotation, vec3 scale);
    virtual ~Ship();
    
//    void drawGlow();
//    
//    // Getters
//    
//    // Setters
//    void setLightPos(vec3 lightPos);
//    void setLightDir(vec3 lightDir);
    
    virtual void update(float dt);
    virtual void render();
};

#endif /* defined(__ex3__Ship__) */
