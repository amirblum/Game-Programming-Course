//
//  InputManager.cpp
//  CGP-Ex2
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include "InputManager.h"

#include <iostream>

/**
 * Constructor. Currently empty
 */
InputManager::InputManager() {}

/**
 * Save a keydown event
 */
void InputManager::handleKeyDown(unsigned char key, int x, int y)
{
    _keyPressed[key] = true;
    _modifier = glutGetModifiers();
}

/**
 * Save a keyup event
 */
void InputManager::handleKeyUp(unsigned char key, int x, int y)
{
    _keyPressed[key] = false;
    _keyQueriedWhilePressed[key] = false;
}

/**
 * Query if a key is pressed
 */
bool InputManager::isPressed(unsigned char key)
{
    return _keyPressed[key];
}

/**
 * Query if a key has been pressed and if it is the first time someone is
 * querying this. Useful for one-off events that shouldn't happen over and
 * over as the key is held down, such as crouching or jumping.
 */
bool InputManager::isPressedFirstTime(unsigned char key)
{
    bool firstTime = _keyPressed[key] && !_keyQueriedWhilePressed[key];
    
    if (firstTime)
    {
        _keyQueriedWhilePressed[key] = true;
    }
    
    return firstTime;
}

/**
 * Query whether a modifier key was pressed (doesn't matter which)
 */
bool InputManager::isModifierPressed()
{
    return _modifier;
}