//
//  CameraFollow.h
//  CGP-Superhero
//
//  Script to force the camera to follow the superhero
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__CameraFollow__
#define __CGP_Superhero__CameraFollow__

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Superhero.h"

class CameraFollow : public Script {
private:
    Camera *_camera;
    Superhero *_superhero;
    
    vec3 _previousFromSuperhero;
    
public:
    CameraFollow(Camera *camera, Superhero *superhero);
    virtual ~CameraFollow();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Superhero__CameraFollow__) */
