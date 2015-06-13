//
//  CameraFollow.h
//  CGP-Ex4
//
//  Script to force the camera to follow the ship
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__CameraFollow__
#define __CGP_Ex4__CameraFollow__

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Ship.h"
#include "SkyBox.h"

class CameraFollow : public Script {
private:
    Camera *_camera;
    Ship *_ship;
    
    vec3 _previousFromShip;
    
    // Also, handle skyboxiness here
    SkyBox *_skybox;
    
public:
    CameraFollow(Camera *camera, Ship *ship, SkyBox *skybox);
    virtual ~CameraFollow();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Ex4__CameraFollow__) */
