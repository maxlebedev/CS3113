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
    //distance = velocity * time
    x += xVelocity * timeElapsed;
    y += yVelocity * timeElapsed;
    
}
//TODO: modelmatrix?
void Ball::Draw(ShaderProgram program,GLuint ballTextureID){
    float ballSize = width;//0.045f;
    /*float vertices[] = {-1*ballSize, -1*ballSize, 1*ballSize, 1*ballSize, -1*ballSize, 1*ballSize, 1*ballSize, 1*ballSize, -1*ballSize, -1*ballSize, 1*ballSize, -1*ballSize};
    vertices[0] += x;
        vertices[1] += y;
    vertices[2] += x;
        vertices[3] += y;
    vertices[4] += x;
        vertices[5] += y;
    vertices[6] += x;
        vertices[7] += y;
    vertices[8] += x;
        vertices[9] += y;
    vertices[10] += x;
        vertices[11] += y;*/
    
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


void Ball::hitTop(){
    yVelocity = -std::abs(yVelocity);
}

void Ball::hitBottom(){
    yVelocity = std::abs(yVelocity);
}

void Ball::hitRightPaddle(int paddleY,int paddleheight){
    float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    std::cout << "relativeIntersectY: " << relativeIntersectY << std::endl;
    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    std::cout << "bounceAngle: " << bounceAngle << std::endl;
    //TODO use bounceangle look up slides
    xVelocity = -1.0f * (((float)rand()) / (float)RAND_MAX);
    yVelocity = 1.5f;

}

void Ball::hitLeftPaddle(int paddleY,int paddleheight){
    float relativeIntersectY = (paddleY+(paddleheight/2)) - y;
    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleheight/2));
    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
    std::cout << "bounceAngle: " << bounceAngle << std::endl;
    xVelocity = 1.0f * (((float)rand()) / (float)RAND_MAX);
    yVelocity = 1.5f;
    
}

//LBRT order
bool Ball::collidesWith(const std::array<float, 4> &rect){
    if(x > (x+width) || rect[1] > (y+height) || (rect[0]+rect[2]) < x || (rect[0]+rect[3]) < y){
        return false;
    }
    cout << "hit: " << rect[0] << rect[1] << rect[2] << rect[3] <<endl;
    return true;
}

//TODO fix this
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
    cout << "xVelocity: " << xVelocity <<endl;
}

void Ball::reset(){
    x = 0.0f;
    y = 0.0f;
    xVelocity = 0.0f;
    yVelocity = 0.0f;
}
