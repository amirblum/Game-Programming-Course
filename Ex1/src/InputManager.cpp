//
//  InputManager.cpp
//  CGP-Ex1
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "InputManager.h"

#include <algorithm>

/**
 * Constructor. Currently empty
 */
InputManager::InputManager() {}

/**
 * Register a new receiver
 */
void InputManager::registerReceiver(InputReceiver *receiver)
{
    _receivers.push_back(receiver);
}

/**
 * Unregister a receiver
 */
void InputManager::unregisterReceiver(InputReceiver *receiver)
{
    std::vector<InputReceiver*>::iterator it;
    it = std::find(_receivers.begin(), _receivers.end(), receiver);
    
    if (it != _receivers.end()) {
        _receivers.erase(it);
    }
}

/**
 * Forward the input to all the registered objects
 */
void InputManager::handleInput(unsigned char key, int x, int y)
{
    std::vector<InputReceiver*>::iterator it;
    for (it = _receivers.begin(); it != _receivers.end(); ++it) {
        InputReceiver *receiver = *it;
        receiver->receiveInput(key, x, y);
    }
}
