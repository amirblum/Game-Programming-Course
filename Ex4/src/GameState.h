//
//  GameState.h
//  CGP-Ex4
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex4__GameState__
#define __CGP_Ex4__GameState__

#include <stdio.h>

struct GameState {
    // Singleton implementation
    static GameState& Instance() {
        static GameState instance;
        return instance;
    }
    
    // These should definitely be an enum. Maybe in Ex5 :P
    bool gameStarted;
    bool gameWon;
    bool gameOver;
    
    void reset();
    
private:
    // Singleton private instantiation
    GameState();
    GameState(GameState const&);
    void operator=(GameState const&);
};

#endif /* defined(__CGP_Ex4__GameState__) */
