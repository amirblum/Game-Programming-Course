//
//  InputManager.h
//  CGP-Ex2
//
//  Singleton InputManager that registers keys pressed
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex2__InputManager__
#define __CGP_Ex2__InputManager__

#include <map>

#include <glm/glm.hpp>
using namespace glm;

// Key definitions
#define KEY_FORWARD ('w')
#define KEY_BACKWARD ('s')
#define KEY_STRAFE_LEFT ('a')
#define KEY_STRAFE_RIGHT ('d')
#define KEY_TURN_LEFT ('q')
#define KEY_TURN_RIGHT ('e')
#define KEY_FLICKER ('f')
#define KEY_BUMP_TOGGLE ('b')
#define KEY_GLOW_TOGGLE ('g')
#define KEY_LIGHTEN ('-')
#define KEY_DARKEN1 ('=')
#define KEY_DARKEN2 ('+')

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
    void handleMouseMove(float x, float y);
    
    // Query functions
    bool isPressed(unsigned char key);
    bool isPressedFirstTime(unsigned char key);
    bool isModifierPressed();
    vec2 getMousePos();
    
private:
    std::map<unsigned char, bool> _keyPressed;
    std::map<unsigned char, bool> _keyQueriedWhilePressed;
    bool _modifier;
    vec2 _mousePos;
    
    // Singleton private instantiation
    InputManager();
    InputManager(InputManager const&);
    void operator=(InputManager const&);
};

#endif /* defined(__CGP_Ex2__InputManager__) */
