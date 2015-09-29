//
//  Paddle.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Paddle.hpp"
#include <iostream>
using namespace std;

Paddle::Paddle(float x, float y, float width, float height):
x(x), y(y), width(width), height(height), xVelocity(0.0f), yVelocity(0.0f), xAcceleration(0.0f), yAcceleration(0.0f){}

void Paddle::Update(float timeElapsed){
    y += yVelocity * timeElapsed;
    yAcceleration *= 0.99;
}

void Paddle::Draw(){
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(x, y, 0);
    glScalef(1.0, 1.0, 1.0);
    
    
    //TODO position of these things waay off
    GLfloat points[] = { -width / 2, -height / 2,width / 2, height / 2,width / 2, -height / 2,-width / 2, -height / 2, width / 2, height / 2, -width / 2, height / 2};
//    TL -width / 2, height / 2,
//    BL -width / 2, -height / 2,
//    TR, width / 2, -height / 2,
//    BR width / 2, height / 2
        
    std::cout << "y: " << y << " x: "<< x << " width: "<< width << " height: " <<height << std::endl;

    glVertexPointer(2, GL_FLOAT, 0, points);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
}

void Paddle::Up(){
    yVelocity = 1.5f;
    if (yAcceleration == 0.0f){
        yAcceleration = -1.0f;
    }
}

void Paddle::Down(){
    yVelocity = -1.5f;
    if (yAcceleration == 0.0f){
        yAcceleration = 1.0f;
    }
}

void Paddle::Stop(){
    yAcceleration = 0.0f;
    if (yVelocity != 0.0f){
        if (y > 1.0f - height / 2){
            y = 1.0f - height / 2;
        }
        else if (y < -1.0f + height / 2){
            y = -1.0f + height / 2;
        }
        yVelocity = 0.0f;
    }
}

float Paddle::getSpin() const{
    return yAcceleration;
}
