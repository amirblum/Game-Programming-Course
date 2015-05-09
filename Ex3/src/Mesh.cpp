#include <assert.h>
#include "Mesh.h"

Mesh::MeshEntry::MeshEntry(std::vector<Vertex> vertices,
                           std::vector<GLuint> indices,
                           std::vector<TextureComponent*> textures) :
RenderComponent("MeshShader")
{
    std::vector<GLfloat> positions;
    for (Vertex &vertex : vertices) {
        vec3 position = vertex.position;
        positions.push_back(position.x);
        positions.push_back(position.y);
        positions.push_back(position.z);
        positions.push_back(1.0f);
    }
    setVBO(positions);
    setIBO(indices);
    
    for (TextureComponent *texture : textures) {
        addTexture(texture);
    }
}

Mesh::MeshEntry::~MeshEntry()
{
}

Mesh::Mesh(std::string filename) :
_entries(), _textures()
{
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate);
    
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
        initMesh(i, mesh);
    }
}


Mesh::~Mesh()
{
}

void Mesh::initMesh(unsigned int index, const aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureComponent*> textures;

    const aiVector3D zero(0.0f, 0.0f, 0.0f);
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D* pPos      = &(mesh->mVertices[i]);
        const aiVector3D* pNormal   = mesh->HasNormals() ? &(mesh->mNormals[i]) : &zero;
        const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;

        Vertex v(vec3(pPos->x, pPos->y, pPos->z),
                 vec2(pTexCoord->x, pTexCoord->y),
                 vec3(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) {
        const aiFace& face = mesh->mFaces[i];
//        assert(face.mNumIndices == 3);
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
                std::string fullPath = dir + "/" + path.data;
                _textures[i] = TextureComponent::create2DTexture(fullPath);
            }
        }
    }
}

void Mesh::render(mat4 worldTransform)
{
    for (MeshEntry *entry : _entries) {
        entry->render(worldTransform);
    }
    
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//
//    for (unsigned int i = 0; i < _entries.size(); i++) {
//        glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
//        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);
//
//        const unsigned int materialIndex = _entries[i]->getMaterialIndex();
//
//        if (materialIndex < m_Textures.size() && m_Textures[materialIndex]) {
//            m_Textures[materialIndex]->Bind(GL_TEXTURE0);
//        }
//
//        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
//        
//    }
//
//    glDisableVertexAttribArray(0);
//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(2);
}
