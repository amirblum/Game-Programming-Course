//
//  HealthBar.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/5/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__HealthBar__
#define __CGP_Ex3__HealthBar__

#include <stdio.h>
#include "RenderableSceneNode.h"
#include "RenderComponent.h"

class HealthBar : public RenderableSceneNode {
private:
    int _maxUnits, _currentUnits;
    
public:
    HealthBar(int maxUnits, vec3 position, quat rotation, vec3 scale);
    virtual ~HealthBar();
    
    int getCurrentUnits();
    void setCurrentUnits(int currentUnits);
    
    virtual void preRender();
};

#endif /* defined(__CGP_Ex3__HealthBar__) */
