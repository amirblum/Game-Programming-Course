//
//  InputReceiver.h
//  CGP-Ex1
//  Virtual base class for objects that can react to input.
//
//  Created by Amir Blum on 3/12/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Ex1__InputReceiver__
#define __CGP_Ex1__InputReceiver__

#include <stdio.h>

class InputReceiver {
public:
    virtual void receiveInput(unsigned char key, int x, int y) = 0;
};

#endif /* defined(__CGP_Ex1__InputReceiver__) */
