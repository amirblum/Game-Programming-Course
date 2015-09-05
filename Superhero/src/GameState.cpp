//
//  GameState.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "GameState.h"

GameState::GameState() :
winState(NOT_STARTED), boostState(NONE), blurSteps(0.0f) {}

void GameState::reset()
{
    winState = NOT_STARTED;
    boostState = NONE;
    blurSteps = 0.0f;
}