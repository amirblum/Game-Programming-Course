#include <assert.h>
#include <memory>
#include "Mesh.h"

Mesh::MeshEntry::MeshEntry(std::vector<Vertex> vertices,
                           std::vector<GLuint> indices,
                           std::vector<std::shared_ptr<TextureComponent>> textures) :
RenderComponent("MeshShader")
{
    setPTNVBO(vertices);
    setIBO(indices);
    
    for (std::shared_ptr<TextureComponent> texture : textures) {
        addTexture(texture);
    }
}

Mesh::MeshEntry::~MeshEntry()
{
}

Mesh::Mesh(std::string filename, float unitConversion, vec3 position, quat rotation, vec3 scale) :
SceneNode(position, rotation, scale),
_entries(), _textures(), _boundingBox()
{
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filename.c_str(),
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_FindInstances |
                                             aiProcess_OptimizeGraph |
                                             aiProcess_OptimizeMeshes |
                                             aiProcess_RemoveRedundantMaterials |
                                             aiProcess_FindInvalidData);
    
    if (!scene) {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
        return;
    }
    
    if (scene->mNumMeshes > 0) {
        _entries.resize(scene->mNumMeshes);
    }
    
    if (scene->mNumMaterials > 0) {
        _textures.resize(scene->mNumMaterials);
    }
    
    initMaterials(scene, filename);
    
    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ;i < _entries.size() ;i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        initMesh(i, mesh, unitConversion);
    }
}


Mesh::~Mesh()
{
    for (MeshEntry *meshEntry : _entries) {
        delete meshEntry;
    }
}

void Mesh::initMesh(unsigned int index, const aiMesh* mesh, float unitConversion)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<TextureComponent>> textures;

    const aiVector3D zero(0.0f, 0.0f, 0.0f);
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D* pPos      = &(mesh->mVertices[i]);
        const aiVector3D* pNormal   = mesh->HasNormals() ? &(mesh->mNormals[i]) : &zero;
        const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;

        Vertex v(vec3(pPos->x, pPos->y, pPos->z) * unitConversion,
                 vec2(pTexCoord->x, pTexCoord->y),
                 vec3(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
        _boundingBox.includePoint(v.position);
    }

    for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) {
            continue;
        }
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
    
    if (_textures.size() > mesh->mMaterialIndex && _textures[mesh->mMaterialIndex]) {
        textures.push_back(_textures[mesh->mMaterialIndex]);
    }
    
    _entries[index] = new MeshEntry(vertices, indices, textures);
}

void Mesh::initMaterials(const aiScene* scene, std::string filename)
{
    // Extract the directory part from the file name
    std::string::size_type slashIndex = filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos) {
        dir = ".";
    }
    else if (slashIndex == 0) {
        dir = "/";
    }
    else {
        dir = filename.substr(0, slashIndex);
    }

    // Initialize the materials
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial* material = scene->mMaterials[i];

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string texturePath(path.data);
                std::string::size_type slashIndex = texturePath.find_last_of("/");
                // Fix for windows
                if (slashIndex == std::string::npos) {
                    slashIndex = texturePath.find_last_of("\\");
                }
                
                std::string basename;
                
                if (slashIndex == std::string::npos) {
                    basename = texturePath;
                }
                else {
                    basename = texturePath.substr(slashIndex + 1, texturePath.size());
                }

                std::string fullPath = dir + "/textures/" + basename;
                _textures[i] = TextureComponent::create2DTexture(fullPath);
            }
        }
    }
}

void Mesh::render()
{
    for (MeshEntry *entry : _entries) {
        entry->setUniform<mat4, UNIFORM_MAT4>("normalRotation", toMat4(getWorldRotation()));
        entry->render(_worldTransform);
    }
}

BoundingBox& Mesh::getBoundingBox() {
    return _boundingBox;
}
