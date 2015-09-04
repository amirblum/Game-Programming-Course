//
//  GameState.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__GameState__
#define __CGP_Superhero__GameState__

#include <stdio.h>

enum WinState {
    NOT_STARTED,
    STARTED,
    WON,
    LOST
};

struct GameState {
    // Singleton implementation
    static GameState& Instance() {
        static GameState instance;
        return instance;
    }
    
    WinState winState;
    bool zooming;
    
    void reset();
    
private:
    // Singleton private instantiation
    GameState();
    GameState(GameState const&);
    void operator=(GameState const&);
};

#endif /* defined(__CGP_Superhero__GameState__) */
