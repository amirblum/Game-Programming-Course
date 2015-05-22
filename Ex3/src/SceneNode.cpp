//
//  SceneNode.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "SceneNode.h"
#include <algorithm>

/**
 * Constructor
 */
SceneNode::SceneNode(vec3 position, quat rotation, vec3 scale) :
_childNodes(0), _parentNode(NULL),
_position(position), _rotation(rotation), _scale(scale),
_positionInvariant(false), _rotationInvariant(false), _scaleInvariant(false),
_localTransform(1.0f), _worldTransform(1.0f),
_visible(true)
{
    rebuildTransforms(true);
}

/**
 * Destructor
 */
SceneNode::~SceneNode()
{
    for (Script *script : _scripts) {
        delete script;
    }
    
    for (SceneNode *child : _childNodes) {
        delete child;
    }
}

/**
 * Add a script to this node
 */
void SceneNode::addScript(Script *script)
{
    _scripts.push_back(script);
}

/**
 * Add a child to the tree under this node
 */
void SceneNode::addChild(SceneNode *child)
{
    _childNodes.push_back(child);
    child->setParent(this);
    child->rebuildTransforms(false);
}

/**
 * Remove a child by index
 */
void SceneNode::removeChild(unsigned int i)
{
    if (i >= _childNodes.size()) {
        return;
    }
    
    delete _childNodes[i];
    _childNodes.erase(_childNodes.begin() + i);
}

/**
 * Remove a child by value
 */
void SceneNode::removeChild(SceneNode *childToRemove)
{
    auto childIterator = std::find(_childNodes.begin(), _childNodes.end(), childToRemove);
    if (childIterator != _childNodes.end()) {
        _childNodes.erase(childIterator);
        delete childToRemove;
    }
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
 * Get world position
 */
vec3 SceneNode::getWorldPosition() const
{
    vec4 affinePosition = vec4(vec3(0.0f), 1.0f);
    vec4 worldPosition = _worldTransform * affinePosition;
    return vec3(worldPosition);
}

/**
 * Get world rotation
 */
quat SceneNode::getWorldRotation() const
{
    return toQuat(_worldTransform);
}

/**
 * Get world scale
 */
vec3 SceneNode::getWorldScale() const
{
    vec4 worldScale = _worldTransform * vec4(1.0f);
    return vec3(worldScale);
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

void SceneNode::setVisibility(bool visible)
{
    _visible = visible;
}

void SceneNode::togglePositionInvariance()
{
    _positionInvariant = !_positionInvariant;
}

void SceneNode::toggleRotationInvariance()
{
    _rotationInvariant = !_rotationInvariant;
}

void SceneNode::toggleScaleInvariance()
{
    _scaleInvariant = !_scaleInvariant;
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
        if (!(_positionInvariant || _rotationInvariant || _scaleInvariant)) {
            _worldTransform = _parentNode->getWorldTransform() * _localTransform;
        } else {
            mat4 parentInvariantTransform(1.0f);
            if (!_scaleInvariant) {
                parentInvariantTransform = scale(mat4(1.0f), _parentNode->getWorldScale()) * parentInvariantTransform;
            }
            if (!_rotationInvariant) {
                parentInvariantTransform = toMat4(_parentNode->getWorldRotation()) * parentInvariantTransform;
            }
            if (!_positionInvariant) {
                parentInvariantTransform = translate(mat4(1.0f), _parentNode->getWorldPosition());
            }
            _worldTransform = parentInvariantTransform * _localTransform;
        }
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
    // Update all scripts
    for (Script *script : _scripts) {
        script->update(dt);
    }
    
    // Update all children
    for (SceneNode *child : _childNodes) {
        child->recursiveUpdate(dt);
    }
    
    // Update myself
    update(dt);
}

/**
 * First, call virtual render method on self to perform any of the node's
 * personal renders, then recursively call render on all the children.
 */
void SceneNode::recursiveRender()
{
    if (!_visible) {
        return;
    }
    
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
