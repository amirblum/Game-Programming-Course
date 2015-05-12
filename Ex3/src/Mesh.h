//
//  Mesh.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__Mesh__
#define __CGP_Ex3__Mesh__

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

class Mesh : public SceneNode {
private:
    class MeshEntry : public RenderComponent {
    public:
        MeshEntry(std::vector<Vertex> vertices,
                  std::vector<GLuint> indices,
                  std::vector<TextureComponent*> textures);
        virtual ~MeshEntry();
    };

    std::vector<MeshEntry*> _entries;
    std::vector<TextureComponent*> _textures;
    
    void initMesh(unsigned int index, const aiMesh* mesh);
    void initMaterials(const aiScene* scene, std::string filename);
    
    
public:
    Mesh(std::string filename,
         vec3 position = vec3(0.0f),
         quat rotation = quat(vec3(0.0f)),
         vec3 scale = vec3(1.0f));
    virtual ~Mesh();
    
    virtual void render();
};


#endif	/* defined(__CGP_Ex3__Mesh__) */

