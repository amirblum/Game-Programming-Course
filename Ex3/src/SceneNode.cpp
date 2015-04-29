//
//  SceneNode.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "SceneNode.h"

/**
 * Constructor
 */
SceneNode::SceneNode(vec3 position, quat rotation, vec3 scale) :
_childNodes(0), _parentNode(NULL),
_position(position), _rotation(rotation), _scale(scale),
_localTransform(1.0f), _worldTransform(1.0f)
{
    rebuildTransforms(true);
}

/**
 * Destructor
 */
SceneNode::~SceneNode()
{
    for (SceneNode *child : _childNodes) {
        delete child;
    }
}

/**
 * Add a child to the tree under this node
 */
void SceneNode::addChild(SceneNode *newChild)
{
    _childNodes.push_back(newChild);
    newChild->setParent(this);
    newChild->rebuildTransforms(false);
}

/**
 * Set this node's parent
 */
void SceneNode::setParent(SceneNode *parentNode)
{
    _parentNode = parentNode;
}

/**
 * Get position
 */
vec3 SceneNode::getPosition() const
{
    return _position;
}

/**
 * Get rotation
 */
quat SceneNode::getRotation() const
{
    return _rotation;
}

/**
 * Get scale
 */
vec3 SceneNode::getScale() const
{
    return _scale;
}

/**
 * Get local transformation
 */
mat4 SceneNode::getLocalTransform() const
{
    return _localTransform;
}

/**
 * Get world transformation
 */
mat4 SceneNode::getWorldTransform() const
{
    return _worldTransform;
}

/**
 * Set position
 */
void SceneNode::setPosition(vec3 position)
{
    _position = position;
    rebuildTransforms(true);
}

/**
 * Get rotation
 */
void SceneNode::setRotation(quat rotation)
{
    _rotation = rotation;
    rebuildTransforms(true);
}

/**
 * Get scale
 */
void SceneNode::setScale(vec3 scale)
{
    _scale = scale;
    rebuildTransforms(true);
}

/**
 * Rebuild the local transform, then call rebuild on all children recursively.
 * localChanged: If this is false it means only the parent changed, so no
 *               need to rebuild localTransform, only world.
 */
void SceneNode::rebuildTransforms(bool localChanged)
{
    // Rebuild local transform
    if (localChanged) {
        _localTransform = mat4(1.0f);
        _localTransform = scale(mat4(1.0f), _scale) * _localTransform;
        _localTransform = toMat4(_rotation) * _localTransform;
        _localTransform = translate(mat4(1.0f), _position) * _localTransform;
    }
    
    // Rebuild world transform
    if (_parentNode) {
        _worldTransform = _parentNode->getWorldTransform() * _localTransform;
    }
    
    // Update all the kiddies
    for (SceneNode *child : _childNodes) {
        child->rebuildTransforms(false);
    }
}

/**
 * First, call virtual update method on self to perform any of the node's
 * personal updates, then recursively call update on all the children.
 */
void SceneNode::recursiveUpdate(float dt)
{
    // Update myself
    update(dt);
    
    // Update all children
    for (SceneNode *child : _childNodes) {
        child->recursiveUpdate(dt);
    }
}

/**
 * First, call virtual render method on self to perform any of the node's
 * personal renders, then recursively call render on all the children.
 */
void SceneNode::recursiveRender()
{
    // Render myself
    render();
    
    // Render all children
    for (SceneNode *child : _childNodes) {
        child->recursiveRender();
    }
}

/**
 * Empty implementation for personal update
 */
void SceneNode::update(float dt) {}

/**
 * Empty implementation for personal render
 */
void SceneNode::render() {}