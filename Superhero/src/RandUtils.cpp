//
//  RandUtils.cpp
//  CGP-Superhero
//
//  Created by Amir Blum on 5/11/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include <stdio.h>
#include <random>
#include "RandUtils.h"

namespace randutils {
    float randomRange(float from, float to)
    {
        return ((float)rand() / RAND_MAX) * (to - from) + from;
    }
}