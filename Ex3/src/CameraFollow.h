//
//  CameraFollow.h
//  CGP-Ex3
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__CameraFollow__
#define __CGP_Ex3__CameraFollow__

#include <stdio.h>
#include "Camera.h"
#include "Ship.h"

class CameraFollow {
private:
    Camera *_camera;
    Ship *_ship;
    
public:
    CameraFollow(Camera *camera, Ship *ship);
    virtual ~CameraFollow();
    
    void update(float dt);
};

#endif /* defined(__CGP_Ex3__CameraFollow__) */
