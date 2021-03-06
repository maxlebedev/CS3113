//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Max Lebedev on 10/4/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SheetSprite.hpp"

#define FIXED_TIMESTEP 0.0166666f
#define COLLISION_OFFSET 0.001f

//types
# define PLATFORM 0
# define PLAYER 1
# define BULLET 2
# define BPLATFORM 3
# define ENEMY 4

# define GRAVITY -9.8f

class Entity{
public:
    void Draw(ShaderProgram *program);
    Entity(float x, float y, float width, float height);
    void Render(ShaderProgram *program);
    void Update(float elapsed);
    bool collidesWith(Entity other);
    void uncollide(Entity other);
    float x;
    float y;
    float width;
    float height;
    int type;
    SheetSprite sprite;
    float velocity_x;
    float velocity_y;
    float friction;
    float acceleration_x;
    float acceleration_y;
    
    float bottom();
    float top();
    float left();
    float right();
    float xmid();
    float ymid();
    bool isAlive;
    bool isStatic;
    
    bool leftContact;
    bool rightContact;
//    bool topContact;
    bool bottomContact;
    
    bool gravity;
    bool godmode;
    Entity* player;//enemies need a player to chase
    
};

#endif /* Entity_hpp */
