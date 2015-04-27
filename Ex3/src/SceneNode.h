//
//  SceneNode.h
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__SceneNode__
#define __CGP_Ex3__SceneNode__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

using namespace glm;

#include <stdio.h>
#include <vector>

class SceneNode {
protected:
    // Graph-related
    std::vector<SceneNode*> _childNodes;
    SceneNode *_parentNode;
    
    // Transform-related
    vec3 _position, _rotation, _scale;
    mat4 _localTransform, _worldTransform;
    void rebuildTransforms(bool localChanged);
    
public:
    SceneNode(vec3 position, vec3 rotation, vec3 scale);
    virtual ~SceneNode();
    
    void addChild(SceneNode *newChild);
    void setParent(SceneNode *parentNode);
    
    vec3 getPosition() const;
    vec3 getRotation() const;
    vec3 getScale() const;
    
    mat4 getLocalTransform() const;
    mat4 getWorldTransform() const;
    
    void setPosition(vec3 position);
    void setRotation(vec3 rotation);
    void setScale(vec3 scale);
    
    void recursiveUpdate(float dt);
    void recursiveRender();
    virtual void update(float dt);
    virtual void render();
};

#endif /* defined(__CGP_Ex3__SceneNode__) */
