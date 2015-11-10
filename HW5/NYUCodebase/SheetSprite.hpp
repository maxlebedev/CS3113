//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Max Lebedev on 10/10/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp

#include <stdio.h>
#include "ShaderProgram.h"


class SheetSprite {
public:
    SheetSprite();
    SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
    void Draw(ShaderProgram* program);
    void Draw(ShaderProgram* program, float vertices[]);
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
};

#endif /* SheetSprite_hpp */
