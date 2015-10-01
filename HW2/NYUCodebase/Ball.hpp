//
//  Ball.hpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>

#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"

#include <iostream>
using namespace std;

#include <array>
//#include <math.h>
#include <cmath>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

# define PI           3.14159265358979323846  /* pi */
#define MAXBOUNCEANGLE 5*PI/12

using std::array;

class Ball{
public:
    Ball(float x, float y, float height, float width);
    
    
    void Update(float elapsed);
//    void Draw(ShaderProgram program,GLuint ballTextureID, Matrix modelMatrix);
    void Draw(ShaderProgram program,GLuint ballTextureID);
    
    void hitRightPaddle(int paddleY, int paddleheight);
    void hitLeftPaddle(int paddleY, int paddleheight);
    void hitTop();
    void hitBottom();
    void reset();
    void launch();
    bool collidesWith(const std::array<float, 4> &rect);

    float x;
    float y;
    
    float width;
    float height;
    
    float top();
    float bottom();
    float left();
    float right();
    
    GLuint ballTextureID;
    
private:
    
    float xVelocity;
    float yVelocity;
};


#endif /* Ball_hpp */
