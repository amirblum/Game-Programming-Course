//
//  Script.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef CGP_Superhero_Script_h
#define CGP_Superhero_Script_h

class Script {
public:
    virtual ~Script() {}
    virtual void fixedUpdate(float dt) {}
    virtual void update(float dt) {}
};


#endif
