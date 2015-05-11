//
//  SceneNode.h
//  CGP-Ex3
//
//  A node of the scene graph. Updates/renders all children in the graph
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__SceneNode__
#define __CGP_Ex3__SceneNode__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

#include <stdio.h>
#include <vector>

#define RIGHT_VECTOR vec3(1.0f, 0.0f, 0.0f)
#define UP_VECTOR vec3(0.0f, 1.0f, 0.0f)
#define FORWARD_VECTOR vec3(0.0f, 0.0f, 1.0f)

class SceneNode {
protected:
    // Graph-related
    std::vector<SceneNode*> _childNodes;
    SceneNode *_parentNode;
    
    // Transform-related
    vec3 _position, _scale;
    quat _rotation;
    mat4 _localTransform, _worldTransform;
    void rebuildTransforms(bool localChanged);
    
public:
    SceneNode(vec3 position = vec3(0.0f),
              quat rotation = quat(vec3(0.0f)),
              vec3 scale = vec3(1.0f));
    virtual ~SceneNode();
    
    void addChild(SceneNode *newChild);
    void removeChild(unsigned int i);
    void removeChild(SceneNode *childToRemove);
    void setParent(SceneNode *parentNode);
    
    // Getters
    vec3 getPosition() const;
    quat getRotation() const;
    vec3 getScale() const;
    
    vec3 getWorldPosition() const;
    quat getWorldRotation() const;
    vec3 getWorldScale() const;
    
    mat4 getLocalTransform() const;
    mat4 getWorldTransform() const;
    
    // Setters
    void setPosition(vec3 position);
    void setRotation(quat rotation);
    void setScale(vec3 scale);
    
    // Recursive actions
    void recursiveUpdate(float dt);
    void recursiveRender();
    virtual void update(float dt);
    virtual void render();
};

#endif /* defined(__CGP_Ex3__SceneNode__) */
