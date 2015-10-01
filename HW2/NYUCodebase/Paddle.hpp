//
//  Paddle.hpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#ifndef Paddle_hpp
#define Paddle_hpp

#define PADDLEHEIGHT 1.0f

#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Matrix.h"
#include "ShaderProgram.h"

#include <array>


using std::array;

class Paddle{
public:
    Paddle(float x, float y, float width, float height);
    
    void Update(float elapsed);
    void Draw(ShaderProgram program,GLuint paddleTextureID);
    
    void Up();
    void Down();
    void Stop();
    float getSpin() const;

    float x;
    float y;
    
    float width;
    float height;
    GLuint paddleTextureID;

    
    float top();
    float bottom();
    float left();
    float right();
    
    
private:
    
    float xVelocity;
    float yVelocity;
    
    float xAcceleration;
    float yAcceleration;
};


#endif /* Paddle_hpp */
