#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include <iostream>

#include "Ball.hpp"
#include "Paddle.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

#define ORTHOTOP 1.0f
#define ORTHOBOT -1.0f
#define ORTHOLEFT -1.33f
#define ORTHORIGHT 1.33f

using std::array;
Ball* ball;
Paddle* paddle;
Paddle* paddle2;

float lastFrameTicks;

bool newGame;

float RandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

//LBRT order
bool collided(const std::array<float, 4> &rect1, const std::array<float, 4> &rect2){
    if(rect2[0] > (rect1[0]+rect1[2]) || rect2[1] > (rect1[1]+rect1[3]) || (rect2[0]+rect2[2]) < rect1[0] || (rect2[0]+rect2[3]) < rect1[1])
        return false;
    return true;
}

void HandleCollisions(){
    const array<float, 4> boxBall = {ball->x, ball->y, ball->width, ball->height};
    const array<float, 4> boxPaddle1 = {paddle->x, paddle->y, paddle->width, paddle->height};
    const array<float, 4> boxPaddle2 = {paddle2->x, paddle2->y, paddle2->width, paddle2->height};
    
    //Top
    if (boxBall[1] + boxBall[3] / 2 >= ORTHOTOP)
        ball->hitTop();
    if (boxPaddle1[1] + boxPaddle1[3] / 2 > ORTHOTOP)
        paddle->Stop();
    if (boxPaddle2[1] + boxPaddle2[3] / 2 > ORTHOTOP)
        paddle2->Stop();
    
    //Bottom
    if (boxBall[1] - boxBall[3] / 2 <= ORTHOBOT)
        ball->hitBottom();
    if (boxPaddle1[1] - boxPaddle1[3] / 2 < ORTHOBOT)
        paddle->Stop();
    if (boxPaddle2[1] - boxPaddle2[3] / 2 < ORTHOBOT)
        paddle2->Stop();
    
    //Right
    if (boxBall[0] - boxBall[2] / 2 >= ORTHORIGHT){
        ball->reset();
        newGame = true;
    }
    
    //Left
    if (boxBall[0] + boxBall[2] / 2 <= ORTHOLEFT){
        ball->reset();
        newGame = true;
    }
    //Paddle 1
    if (collided(boxBall,boxPaddle1))
        ball->hitLeftPaddle(boxPaddle1[3], PADDLEHEIGHT);
    //Paddle 2
    if (collided(boxBall, boxPaddle2))
        ball->hitRightPaddle(boxPaddle2[3],PADDLEHEIGHT);
}

void Setup(){//Ball* ball, Paddle* paddle,Paddle* paddle2
    //Setup SDL
    SDL_Init(SDL_INIT_VIDEO);//SDL_INIT_JOYSTICK
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//640, 360
    
    //Setup OpenGL
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif

    //set up our project matrix
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    
    //background
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black
    
    
    //Setup my entities here!
    ball = new Ball(0.0f, 0.0f, 0.1f, 0.1f);
    paddle = new Paddle(-1.0f, 0.0f, 0.1f, 0.30f);
    paddle2 = new Paddle(1.0f, 0.0f, 0.1f, 0.30f);
    
    lastFrameTicks = 0.0f;
    
    newGame = true;
    
}

void ProcessEvents(bool& done){
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        //Exit
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        
        //Keyboard 2 players ONLY
        else if (event.type == SDL_KEYDOWN){
            //Player 1
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                paddle->Up();
            else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                paddle->Down();
            //Player 2
            if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                paddle2->Up();
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                paddle2->Down();
        }
        
        else if (event.type == SDL_KEYUP){
            //Player 1
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                paddle->Stop();
            else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                paddle->Stop();
            
            //Player 2
            if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                paddle2->Stop();
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                paddle2->Stop();
        }
    }
}

void Update(){
    //Physics
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    ball->Update(elapsed);
    paddle->Update(elapsed);
    paddle2->Update(elapsed);
    
    if (newGame){
        ball->launch();
        newGame = false;
    }
    
    //Collisions
    HandleCollisions();
    
}

void Render(){
    //Clears the screen to the set color
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Setup Transforms Draw 
    ball->Draw();
    paddle->Draw();
    paddle2->Draw();
    
    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[]){
    Setup();
    bool done = false;
    while (!done) {
        ProcessEvents(done);
        Update();
        Render();
    }
    SDL_Quit();
    return 0;
}