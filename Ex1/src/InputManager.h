//
//  InputManager.h
//  CGP-Ex1
//
//  Singleton InputManager that registers keys pressed
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__InputManager__
#define __CGP_Ex1__InputManager__

#include <map>

// Key definitions
#define KEY_FORWARD ('w')
#define KEY_BACKWARD ('s')
#define KEY_TURN_LEFT ('a')
#define KEY_TURN_RIGHT ('d')
#define KEY_JUMP (' ')
#define KEY_STRAFE_LEFT ('q')
#define KEY_STRAFE_RIGHT ('e')
#define KEY_CROUCH ('z')

class InputManager {
public:
    // Singleton implementation
    static InputManager& Instance() {
        static InputManager instance;
        
        return instance;
    }
    
    // Receive input events
    void handleKeyDown(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    
    // Query functions
    bool isPressed(unsigned char key);
    bool isPressedFirstTime(unsigned char key);
    bool isModifierPressed();
    
private:
    std::map<unsigned char, bool> _keyPressed;
    std::map<unsigned char, bool> _keyQueriedWhilePressed;
    bool _modifier;
    
    // Singleton private instantiation
    InputManager();
    InputManager(InputManager const&);
    void operator=(InputManager const&);
};

#endif /* defined(__CGP_Ex1__InputManager__) */
