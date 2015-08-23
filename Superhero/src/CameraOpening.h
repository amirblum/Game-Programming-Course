//
//  CameraOpening.h
//  CGP-Superhero
//
//  Script to force the camera to follow the superhero
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__CameraOpening__
#define __CGP_Superhero__CameraOpening__

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Superhero.h"

class CameraOpening : public Script {
private:
    Camera *_camera;
    Superhero *_superhero;
    
    vec3 _startPosition;
    vec3 _endPosition;
    
    bool _started;
    
public:
    CameraOpening(Camera *camera, Superhero *superhero);
    virtual ~CameraOpening();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Superhero__CameraOpening__) */
