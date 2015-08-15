//
//  CameraScripts.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraScripts.h"
#include "CameraOpening.h"
#include "CameraFollow.h"
#include "CameraEnding.h"

#include "GameState.h"

CameraScripts::CameraScripts(Camera *camera, Superhero *superhero, SkyBox *skyBox) :
_cameraOpening(new CameraOpening(camera, superhero, skyBox)),
_cameraFollow(new CameraFollow(camera, superhero, skyBox)),
_cameraEnding(new CameraEnding(camera, superhero, skyBox))
{
}

CameraScripts::~CameraScripts()
{
    delete _cameraOpening;
    delete _cameraFollow;
    delete _cameraEnding;
}

void CameraScripts::update(float dt)
{
    GameState &state = GameState::Instance();
    
    switch (state.winState) {
        case NOT_STARTED:
            _cameraOpening->update(dt);
            break;
        case STARTED:
            _cameraFollow->update(dt);
            break;
        case WON:
        case LOST:
            _cameraEnding->update(dt);
            break;
    }
}