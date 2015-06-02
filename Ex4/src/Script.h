//
//  Script.h
//  CGP-Ex4
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef CGP_Ex4_Script_h
#define CGP_Ex4_Script_h

class Script {
public:
    virtual ~Script() {}
    virtual void fixedUpdate(float dt) {}
    virtual void update(float dt) {}
};


#endif
