//
//  CameraEnding.h
//  CGP-Ex3
//
//  Script to have the camera go to side-view of ship
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__CameraEnding__
#define __CGP_Ex3__CameraEnding__

#include <stdio.h>

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Ship.h"
#include "SkyBox.h"

class CameraEnding : public Script {
private:
    Camera *_camera;
    Ship *_ship;
    SkyBox *_skyBox;
    
    vec3 _startOffset;
    vec3 _endOffset;
    vec3 _startingUp;
    float _currentTravelTime;
    bool _started;
    
public:
    CameraEnding(Camera *camera, Ship *ship, SkyBox *skyBox);
    virtual ~CameraEnding();
    
    void update(float dt);
};

#endif /* defined(__CGP_Ex3__CameraEnding__) */
