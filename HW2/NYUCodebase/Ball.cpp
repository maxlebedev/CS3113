//
//  Ball.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 9/27/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Ball.hpp"


Ball::Ball(float x, float y, float width, float height)	:x(x), y(y),width(width), height(height), xVelocity(0.0f), yVelocity(0.0f) {
}

void Ball::Update(float timeElapsed){
    x += xVelocity * timeElapsed;
    y += yVelocity * timeElapsed;
    
    //x += cos(angle)*timeElapsed * speed;
    //y += sin(angle)*timeElapsed * speed;
    
}
//TODO: modelmatrix?
void Ball::Draw(ShaderProgram program,GLuint ballTextureID){
    float ballSize = width;
    
    //x,y,x+w,y+h as corners
    float vertices[] = {x, y, x+ballSize, y+ballSize, x, y+ballSize, x+ballSize, y+ballSize, x, y, x+ballSize, y};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    float texCoords[] = {0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

float Ball::top(){
    return y+height;
}

float Ball::bottom(){
    return y;
}

float Ball::left(){
    return x;
}
float Ball::right(){
    return x+width;
}


void Ball::hitTop(){
    //if(angle < (PI/2))
    //    angle -= (PI/2);
    //else
    //    angle += (PI/2);
    yVelocity = -std::abs(yVelocity);
    
}

void Ball::hitBottom(){
    yVelocity = std::abs(yVelocity);
    //if(angle < (-PI/2))
    //    angle += (-PI/2);
    //else
    //    angle -= (-PI/2);
}

void Ball::hitRightPaddle(){
    //float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    //std::cout << "relativeIntersectY: " << relativeIntersectY << std::endl;
    //float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    //float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    //std::cout << "bounceAngle: " << bounceAngle << std::endl;
    //TODO use bounceangle look up slides
    xVelocity = -1.0f * (((float)rand()) / (float)RAND_MAX);
    yVelocity = 1.5f;
    //if(angle < PI )
    //    angle += (PI/2);
    //else
    //    angle -= (PI/2);
    //speed += 0.01f;

}

void Ball::hitLeftPaddle(){
    //if(angle > 0 )
    //    angle -= (PI/2);
    //else
   //     angle += (PI/2);
    //speed += 0.01f;
    
    xVelocity = 1.0f * (((float)rand()) / (float)RAND_MAX);
    yVelocity = 1.5f;
    
}

void Ball::launch(){
    float s1 = (((float)rand()) / (float)RAND_MAX);
    float s2 = (((float)rand()) / (float)RAND_MAX);
    int xsign, ysign;
    if(s1 < 0.5f)
        xsign = -1;
    else
        xsign = 1;
    if(s2 < 0.5f)
        ysign = -1;
    else
        ysign = 1;
    xVelocity = 2.50f*xsign*(((float)rand()) / (float)RAND_MAX);//*r
    yVelocity = 1.5f *ysign*(((float)rand()) / (float)RAND_MAX);
    
    //angle = (float)rand() / (float)(PI*2);
    //speed = 1;
    
}

void Ball::reset(){
    x = 0.0f;
    y = 0.0f;
    xVelocity = 0.0f;
    yVelocity = 0.0f;
}
