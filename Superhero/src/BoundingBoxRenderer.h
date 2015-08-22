//
//  BoundingBoxRenderer.h
//  cg-projects
//
//  Created by Amir Blum on 3/10/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__BoundingBoxRenderer__
#define __CGP_Superhero__BoundingBoxRenderer__

#include "RenderableSceneNode.h"
#include "BoundingBox.h"

class BoundingBoxRenderer : public RenderableSceneNode {
private:
    void initWithBoundingBox(BoundingBox &bb);
public:
    BoundingBoxRenderer(BoundingBox &bb);
    virtual ~BoundingBoxRenderer();
    
    void setBoundingBox(BoundingBox &bb);
    
    virtual void preRender();
};

#endif /* defined(__CGP_Superhero__BoundingBoxRenderer__) */
