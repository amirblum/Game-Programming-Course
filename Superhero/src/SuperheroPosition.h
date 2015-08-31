//
//  SuperheroPosition.h
//  CGP-Superhero
//
//  Script to force the camera to follow the superhero
//
//  Created by Amir Blum on 4/27/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__SuperheroPosition__
#define __CGP_Superhero__SuperheroPosition__

#include <stdio.h>
#include "Script.h"
#include "City.h"
#include "Superhero.h"

class SuperheroPosition : public Script {
private:
    City *_city;
    Superhero *_superhero;
    
public:
    SuperheroPosition(City *_city, Superhero *superhero);
    virtual ~SuperheroPosition();
    
    virtual void update(float dt);
};

#endif /* defined(__CGP_Superhero__SuperheroPosition__) */
