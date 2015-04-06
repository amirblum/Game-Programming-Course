//
//  Corridor.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex2__Corridor__
#define __ex2__Corridor__

#include "Renderable.h"

#include <vector>

#define GRID_ELEMENT_SIZE (1.0f)

class Corridor : public Renderable {
private:
    // Info
    float _width, _height, _length;

public:
    Corridor(float width, float height, float length);
    virtual ~Corridor();
    
    // Getters
    float getWidth();
    float getHeight();
    float getLength();

};

#endif /* defined(__ex2__Corridor__) */