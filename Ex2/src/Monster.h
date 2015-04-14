//
//  Monster.h
//  CGP-Ex2
//
//  Created by Amir Blum on 4/14/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex2__Monster__
#define __CGP_Ex2__Monster__

#include <stdio.h>
#include "Renderable.h"

class Monster : public Renderable {
private:
    GLuint _lightDarkenStartUniform;
    float _lightDarkenStart;
public:
    Monster(float width, float height, vec3 startPosition, float lightDarkenStart);
    void customBindings();
    void moveForward(float dt);
};

#endif /* defined(__CGP_Ex2__Monster__) */
