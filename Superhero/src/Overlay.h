//
//  Overlay.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__Overlay__
#define __CGP_Superhero__Overlay__

#include <stdio.h>
#include "RenderableSceneNode.h"

class Overlay : public RenderableSceneNode {
public:
    Overlay(std::string texture);
    virtual ~Overlay();
};

#endif /* defined(__CGP_Superhero__Overlay__) */
