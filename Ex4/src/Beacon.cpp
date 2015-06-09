//
//  Beacon.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 6/4/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "Beacon.h"

static const std::string BEACON_MESH = "assets/battlestar/battlestar.dae";

Beacon::Beacon(vec3 position, quat rotation, vec3 scale, float radius) :
SceneNode(position, rotation, scale),
RigidBody(position, vec3(0.0f), radius, radius, false)
{
    // Set up physics
    _physics.setStatic(true);
    
    // Set up mesh
    vec3 meshPosition = vec3(0.0f);
    quat meshRotation = rotate(quat(vec3(0.0f)), pi<float>()/2.0f, vec3(0.0f, 0.0f, 1.0f));
    meshRotation = rotate(meshRotation, pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
//    quat meshRotation = quat(vec3(0.0f));
    vec3 meshScale = vec3(1.0f);
    
    _mesh = new Mesh(BEACON_MESH, meshPosition, meshRotation, meshScale);
    addChild(_mesh);
}

Beacon::~Beacon() {}