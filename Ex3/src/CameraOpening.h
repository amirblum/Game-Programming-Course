//
//  CameraOpening.h
//  CGP-Ex3
//
//  Script to force the camera to follow the ship
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__CameraOpening__
#define __CGP_Ex3__CameraOpening__

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Ship.h"
#include "SkyBox.h"

class CameraOpening : public Script {
private:
    Camera *_camera;
    Ship *_ship;
    SkyBox *_skyBox;
    
    vec3 _startPosition;
    vec3 _endPosition;
    
    bool _started;
    
public:
    CameraOpening(Camera *camera, Ship *ship, SkyBox *skyBox);
    virtual ~CameraOpening();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Ex3__CameraOpening__) */
