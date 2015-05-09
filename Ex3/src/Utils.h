//
//  Utils.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/9/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef CGP_Ex3_Utils_h
#define CGP_Ex3_Utils_h

float randomRange(float from, float to)
{
    return ((float)rand() / RAND_MAX) * (to - from) + from;
}

#endif
