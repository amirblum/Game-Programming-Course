//
//  SceneNode.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "SceneNode.h"

SceneNode::SceneNode(vec3 position, vec3 rotation, vec3 scale) :
_childNodes(0), _parentNode(NULL),
_position(position), _rotation(rotation), _scale(scale),
_localTransform(1.0f), _worldTransform(1.0f)
{
    rebuildTransforms(true);
}

SceneNode::~SceneNode()
{
    for (SceneNode *child : _childNodes) {
        delete child;
    }
}

void SceneNode::addChild(SceneNode *newChild)
{
    _childNodes.push_back(newChild);
    newChild->setParent(this);
    newChild->rebuildTransforms(false);
}

void SceneNode::setParent(SceneNode *parentNode)
{
    _parentNode = parentNode;
}

vec3 SceneNode::getPosition() const
{
    return _position;
}

vec3 SceneNode::getRotation() const
{
    return _rotation;
}

vec3 SceneNode::getScale() const
{
    return _scale;
}

mat4 SceneNode::getLocalTransform() const
{
    return _localTransform;
}

mat4 SceneNode::getWorldTransform() const
{
    return _worldTransform;
}

void SceneNode::setPosition(vec3 position)
{
    _position = position;
    rebuildTransforms(true);
}

void SceneNode::setRotation(vec3 rotation)
{
    _rotation = rotation;
    rebuildTransforms(true);
}

void SceneNode::setScale(vec3 scale)
{
    _scale = scale;
    rebuildTransforms(true);
}

void SceneNode::rebuildTransforms(bool localChanged)
{
    // Rebuild local transform
    if (localChanged) {
        _localTransform = mat4(1.0f);
        
        _localTransform = scale(_localTransform, _scale);
        
        vec3 up(0.0f, 1.0f, 0.0f);
        if (_rotation != up) {
            float rotationAmount = acos(dot(_rotation, up));
            vec3 rotationAxis = cross(_rotation, up);
            _localTransform = rotate(_localTransform, rotationAmount, rotationAxis);
        }
        
        _localTransform = translate(_localTransform, _position);
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

void SceneNode::recursiveUpdate(float dt)
{
    // Update myself
    update(dt);
    
    // Update all children
    for (SceneNode *child : _childNodes) {
        child->recursiveUpdate(dt);
    }
}

void SceneNode::recursiveRender()
{
    // Render myself
    render();
    
    // Render all children
    for (SceneNode *child : _childNodes) {
        child->recursiveRender();
    }
}

void SceneNode::update(float dt) {}
void SceneNode::render() {}