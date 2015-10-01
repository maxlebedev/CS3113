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

#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include <stdint.h>
#include <inttypes.h>

#define LEFTBORDER -1.7777f
#define BOTTOMBORDER -1.0f
#define RIGHTBORDER 1.7777f
#define TOPBORDER 1.0f

SDL_Window* displayWindow;
Ball* ball;
Paddle* paddle1;
Paddle* paddle2;
float lastFrameTicks;
bool newGame;
GLuint ballTextureID;
GLuint paddleTextureID;

//LBRT order
bool collided(Ball ball, Paddle paddle){
    if(paddle.top() < ball.bottom() || paddle.bottom() > ball.top() || paddle.left() > ball.right() || paddle.right() < ball.left()){
        return false;
    }
    return true;
}

void HandleCollisions(){
    if (ball->top() >= TOPBORDER)
        ball->hitTop();
    if (paddle1->top() > TOPBORDER)
        paddle1->Stop();
    if (paddle2->top() > TOPBORDER)
        paddle2->Stop();
    
    if (ball->bottom() <= BOTTOMBORDER)
        ball->hitBottom();
    if (paddle1->bottom() < BOTTOMBORDER)
        paddle1->Stop();
    if (paddle2->bottom() < BOTTOMBORDER)
        paddle2->Stop();
    
    if (ball->right() >= RIGHTBORDER){
        ball->reset();
        newGame = true;
    }
    if (ball->left() <= LEFTBORDER){
        ball->reset();
        newGame = true;
    }
    
    if (collided(*ball,*paddle1))
        ball->hitLeftPaddle();
    if (collided(*ball,*paddle2))
        ball->hitRightPaddle();
}

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(0, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

ShaderProgram Setup(){//Ball* ball, Paddle* paddle,Paddle* paddle2
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif

    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glViewport(0, 0, 800, 600);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    ballTextureID = LoadTexture(RESOURCE_FOLDER"ballBlue.png");
    ball = new Ball(-0.15f, -0.15f, 0.15f, 0.15f);
    paddleTextureID = LoadTexture(RESOURCE_FOLDER"brickWall.png");
    
    paddle1 = new Paddle(-1.5f, 0.0f, 0.15f, 0.30f);
    paddle2 = new Paddle(1.5f, 0.0f, 0.15f, 0.30f);

    
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);

    glUseProgram(program.programID);
    
    lastFrameTicks = 0.0f;
    newGame = true;
    return program;
}

void ProcessEvents(bool& done){
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        
        else if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                paddle1->Up();
            else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                paddle1->Down();
            if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                paddle2->Up();
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                paddle2->Down();
        }
        else if (event.type == SDL_KEYUP){
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                paddle1->Stop();
            else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                paddle1->Stop();
            if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                paddle2->Stop();
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                paddle2->Stop();
        }
    }
}

/*
void input(const Uint8 *keys){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_W])
        paddle1->Up();
    else if(keys[SDL_SCANCODE_S])
        paddle1->Down();
    
    if(keys[SDL_SCANCODE_UP])
        paddle2->Up();
    else if(keys[SDL_SCANCODE_DOWN])
        paddle1->Down();
}*/


void Update(){
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    ball->Update(elapsed);
    paddle1->Update(elapsed);
    paddle2->Update(elapsed);
    
    if (newGame){
        ball->launch();
        newGame = false;
    }
    HandleCollisions();
}

void Render(ShaderProgram program){
    glClear(GL_COLOR_BUFFER_BIT);
    
    ball->Draw(program, ballTextureID);
    paddle1->Draw(program, paddleTextureID);
    paddle2->Draw(program, paddleTextureID);
    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[]){
    ShaderProgram program = Setup();
    bool done = false;
    while (!done) {
        ProcessEvents(done);
        Update();
        Render(program);
        
        glUseProgram(program.programID);
    }
    SDL_Quit();
    return 0;
}