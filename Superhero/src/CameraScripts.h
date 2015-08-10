//
//  CameraScripts.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__CameraScripts__
#define __CGP_Superhero__CameraScripts__

#include <stdio.h>
#include "Script.h"
#include "Camera.h"
#include "Ship.h"
#include "SkyBox.h"
#include <vector>

class CameraScripts : public Script {
private:
    Script *_cameraOpening;
    Script *_cameraFollow;
    Script *_cameraEnding;
public:
    CameraScripts(Camera *camera, Ship *ship, SkyBox *skyBox);
    virtual ~CameraScripts();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Superhero__CameraScripts__) */
