//
//  Mesh.h
//  CGP-Superhero
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

#include <map>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "SceneNode.h"
#include "RenderComponent.h"
#include "BoundingBox.h"

class Mesh : public SceneNode {
private:
    class MeshEntry : public RenderComponent {
    public:
        MeshEntry(std::vector<Vertex> vertices,
                  std::vector<GLuint> indices,
                  std::vector<std::shared_ptr<TextureComponent>> textures);
        virtual ~MeshEntry();
    };

    std::vector<MeshEntry*> _entries;
    std::vector<std::shared_ptr<TextureComponent>> _textures;
    BoundingBox _boundingBox;
    
    void initMesh(unsigned int index, const aiMesh* mesh, float unitConversion);
    void initMaterials(const aiScene* scene, std::string filename);
    
    
public:
    Mesh(std::string filename,
         float unitConversion,
         vec3 position = vec3(0.0f),
         quat rotation = quat(vec3(0.0f)),
         vec3 scale = vec3(1.0f));
    virtual ~Mesh();
    
    virtual void render();
    BoundingBox& getBoundingBox();
};


#endif	/* defined(__CGP_Superhero__Mesh__) */

