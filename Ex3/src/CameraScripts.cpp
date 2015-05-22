//
//  CameraScripts.cpp
//  CGP-Ex3
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "CameraScripts.h"
#include "CameraOpening.h"
#include "CameraFollow.h"
#include "CameraEnding.h"

#include "GameState.h"

CameraScripts::CameraScripts(Camera *camera, Ship *ship, SkyBox *skyBox) :
_cameraOpening(new CameraOpening(camera, ship, skyBox)),
_cameraFollow(new CameraFollow(camera, ship, skyBox)),
_cameraEnding(new CameraEnding(camera, ship, skyBox))
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
    
    if (!state.gameStarted) {
        _cameraOpening->update(dt);
    } else if (!state.gameOver) {
        _cameraFollow->update(dt);
    } else {
        _cameraEnding->update(dt);
    }
}