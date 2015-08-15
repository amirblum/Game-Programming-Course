//
//  CameraEnding.h
//  CGP-Superhero
//
//  Script to have the camera go to side-view of superhero
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__CameraEnding__
#define __CGP_Superhero__CameraEnding__

#include <stdio.h>

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Superhero.h"
#include "SkyBox.h"

class CameraEnding : public Script {
private:
    Camera *_camera;
    Superhero *_superhero;
    SkyBox *_skyBox;
    
    vec3 _startOffset;
    vec3 _endOffset;
    vec3 _startingUp;
    float _currentTravelTime;
    bool _started;
    
public:
    CameraEnding(Camera *camera, Superhero *superhero, SkyBox *skyBox);
    virtual ~CameraEnding();
    
    void update(float dt);
};

#endif /* defined(__CGP_Superhero__CameraEnding__) */
