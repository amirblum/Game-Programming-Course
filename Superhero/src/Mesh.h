//
//  Mesh.h
//  CGP-Superhero
//
//  Wrapper class for meshes with added features such as drawing a bounding box.
//  The mesh will be centered within this wrappers coordinates.
//
//  Created by Amir Blum on 5/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__Mesh__
#define __CGP_Superhero__Mesh__

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
using namespace glm;

#include "SceneNode.h"
#include "MeshRenderer.h"
#include "BoundingBoxRenderer.h"

class Mesh : public SceneNode {
private:
    MeshRenderer *_mesh;
    bool _drawBoundingBox;
    BoundingBoxRenderer *_bbRenderer;
    
public:
    Mesh(std::string filename,
         float unitConversion,
         vec3 position = vec3(0.0f),
         quat rotation = quat(vec3(0.0f)),
         vec3 scale = vec3(1.0f));
    virtual ~Mesh();
    
    virtual void update(float dt);
    BoundingBox& getBoundingBox();
    void setCustomBoundingBox(BoundingBox bb);
    void drawBoundingBox(bool draw);
    
    void stretchToFill(vec3 size);
};


#endif	/* defined(__CGP_Superhero__Mesh__) */

