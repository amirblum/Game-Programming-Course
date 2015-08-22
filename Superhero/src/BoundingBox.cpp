//
//  BoundingBox.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 8/15/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox() :
_min(0.0f), _max(0.0f) {}

BoundingBox::BoundingBox(vec3 min, vec3 max) :
_min(min), _max(max) { }

BoundingBox::~BoundingBox() {}

void BoundingBox::includePoint(vec3 point) {
    _min.x = glm::min(_min.x, point.x);
    _min.y = glm::min(_min.y, point.y);
    _min.z = glm::min(_min.z, point.z);
    _max.x = glm::max(_max.x, point.x);
    _max.y = glm::max(_max.y, point.y);
    _max.z = glm::max(_max.z, point.z);
}

float BoundingBox::minX() {
    return _min.x;
}

float BoundingBox::maxX() {
    return _max.x;
}

float BoundingBox::minY() {
    return _min.y;
}

float BoundingBox::maxY() {
    return _max.y;
}

float BoundingBox::minZ() {
    return _min.z;
}

float BoundingBox::maxZ() {
    return _max.z;
}

vec3 BoundingBox::getSize() {
    return _max - _min;
}