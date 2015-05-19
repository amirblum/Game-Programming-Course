//
//  Script.h
//  CGP-Ex3
//
//  Created by Amir Blum on 5/19/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef CGP_Ex3_Script_h
#define CGP_Ex3_Script_h

class Script {
public:
    virtual ~Script() {}
    virtual void update(float dt) = 0;
};


#endif
