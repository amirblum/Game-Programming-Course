//
//  GameState.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex3__GameState__
#define __CGP_Ex3__GameState__

#include <stdio.h>

struct GameState {
    // Singleton implementation
    static GameState& Instance() {
        static GameState instance;
        return instance;
    }
    
    bool gameOver;
    
private:
    // Singleton private instantiation
    GameState();
    GameState(GameState const&);
    void operator=(GameState const&);
};

#endif /* defined(__CGP_Ex3__GameState__) */
