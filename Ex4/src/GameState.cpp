//
//  GameState.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "GameState.h"

GameState::GameState() :
gameStarted(false),
gameWon(false),
gameOver(false) {}

void GameState::reset()
{
    gameStarted = false;
    gameWon = false;
    gameOver = false;
}