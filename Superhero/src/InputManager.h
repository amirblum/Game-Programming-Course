//
//  InputManager.h
//  CGP-Superhero
//
//  Singleton InputManager that registers keys pressed
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__InputManager__
#define __CGP_Superhero__InputManager__

#include <map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
using namespace glm;

// Key definitions
#define KEY_ACTION (' ')
#define KEY_UP ('w')
#define KEY_DOWN ('s')
#define KEY_LEFT ('a')
#define KEY_RIGHT ('d')
#define KEY_LEFT_MOD ('q')
#define KEY_RIGHT_MOD ('e')
#define KEY_GAME_OVER ('g')

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

#endif /* defined(__CGP_Superhero__InputManager__) */
