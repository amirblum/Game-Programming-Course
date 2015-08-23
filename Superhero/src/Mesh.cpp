#include "Mesh.h"
#include "InputManager.h"

Mesh::Mesh(std::string filename, float unitConversion, vec3 position, quat rotation, vec3 scale) :
SceneNode(position, rotation, scale),
_mesh(new MeshRenderer(filename, unitConversion)), _drawBoundingBox(false), _bbRenderer(new BoundingBoxRenderer(_mesh->getBoundingBox()))
{
    _mesh->centerMesh();
    addChild(_mesh);
    addChild(_bbRenderer);
}

Mesh::~Mesh()
{
}

void Mesh::update(float dt)
{
    _bbRenderer->setVisibility(InputManager::Instance().isPressed(KEY_BB));
}

BoundingBox& Mesh::getBoundingBox() {
    return _mesh->getBoundingBox();
}

void Mesh::setCustomBoundingBox(BoundingBox bb) {
    _mesh->setCustomBoundingBox(bb);    
    _bbRenderer->setBoundingBox(bb);
}

void Mesh::drawBoundingBox(bool draw) {
    _drawBoundingBox = draw;
}

// Strech the SceneNode so that the mesh fills a given size. -1 ignores that axis.
void Mesh::stretchToFill(vec3 size) {
    vec3 bbSize = _mesh->getBoundingBox().getSize();
    
    if (size.x == -1.0f) {
        size.x = bbSize.x;
    }
    
    if (size.y == -1.0f) {
        size.y = bbSize.y;
    }
    
    if (size.z == -1.0f) {
        size.z = bbSize.z;
    }
    
    setScale(size / bbSize);
}
