//
//  InputManager.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "InputManager.h"

#include <iostream>

/**
 * Constructor. Currently empty
 */
InputManager::InputManager() {}

void InputManager::handleKeyDown(unsigned char key, int x, int y)
{
    _keyPressed[key] = true;
}

void InputManager::handleKeyUp(unsigned char key, int x, int y)
{
    _keyPressed[key] = false;
    _keyQueriedWhilePressed[key] = false;
}

bool InputManager::isPressed(unsigned char key)
{
    return _keyPressed[key];
}

bool InputManager::isPressedFirstTime(unsigned char key)
{
    bool firstTime = _keyPressed[key] && !_keyQueriedWhilePressed[key];
    
    if (firstTime)
    {
        _keyQueriedWhilePressed[key] = true;
    }
    
    return firstTime;
}