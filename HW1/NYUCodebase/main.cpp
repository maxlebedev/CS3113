#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    if (surface == NULL)
    {
        std::cout << "FAILED TO FIND THE IMAGE" << std::endl;
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation) {

    float UVtexCoords[] = {0.0, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0, 0.0, 1.0, -1.0, 0.0, -1.0};
    float texCoords[] = {0.0, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0, 0.0, 0.5, 0.5, 0.0, 0.5};
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(x, y, 0.0);
//  glScalef(rotation, rotation, 1.0);
    glRotatef(-rotation*rotation, 0.0, 0.0, 1.0);
    
    glVertexPointer(2, GL_FLOAT, 0, texCoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, UVtexCoords);//uvs
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    SDL_Event event;
    bool done = false;
    glViewport(0, 0, 640, 360);
    glMatrixMode(GL_PROJECTION);
    
    GLuint alienId = LoadTexture(RESOURCE_FOLDER"alienGreen.png");
    GLuint blueChipId = LoadTexture(RESOURCE_FOLDER"chipBlueWhite_border.png");
    GLuint blackChipId = LoadTexture(RESOURCE_FOLDER"chipBlackWhite_border.png");

    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	angle += elapsed;
        DrawSprite(alienId, -0.25,y,angle);
        DrawSprite(blueChipId, -1.0,y,0);
        DrawSprite(blackChipId, 0.5,y,0);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
