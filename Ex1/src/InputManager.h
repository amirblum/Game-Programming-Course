//
//  InputManager.h
//  CGP-Ex1
//  Singleton InputManager that InputReceivers can register to to
//  receive input.
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__InputManager__
#define __CGP_Ex1__InputManager__

#include "InputReceiver.h"

#include <vector>

// Key definitions
#define KEY_FORWARD ('w')
#define KEY_BACKWARD ('s')
#define KEY_TURN_LEFT ('a')
#define KEY_TURN_RIGHT ('d')
#define KEY_JUMP (' ')

class InputManager {
public:
    static InputManager& Instance() {
        static InputManager instance;
        
        return instance;
    }
    
    void registerReceiver(InputReceiver *receiver);
    void unregisterReceiver(InputReceiver *receiver);
    void handleInput(unsigned char key, int x, int y);
    
private:
    std::vector<InputReceiver*> _receivers;
    
    InputManager();
    InputManager(InputManager const&);
    void operator=(InputManager const&);
};

#endif /* defined(__CGP_Ex1__InputManager__) */
