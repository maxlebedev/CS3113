#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "vector"
#include "Entity.hpp"
#include "SheetSprite.hpp"
#include "Chunk.hpp"
//#include "SDL_TTF.h"

#include <SDL_mixer.h>
#include <vector>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
int state;
bool done;

// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#define LWALL -1.7
#define RWALL 1.5

#define PLAYER_ENT entities[0]

#define CHUNK_W 20
#define CHUNK_H 10

float timeLeftOver = 0.0f;
float lastFrameTicks = 0.0f;

std::vector<Entity> bullets;

//40 is player
std::vector<Entity> entities;

std::vector<Entity> menuEntities;


SDL_Window* displayWindow;

Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;

Mix_Chunk *pewpew;
Mix_Music *music;

#define LEVEL_HEIGHT 16
#define LEVEL_WIDTH 22
#define TILE_X 0.17f
#define TILE_Y 0.20f
//.23

//TODO rewrite level data
unsigned int levelData[LEVEL_HEIGHT][LEVEL_WIDTH]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};


GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(0, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void DrawText(ShaderProgram* program, int fontTexture, std::string text, float size, float spacing) {
    for(int i=0; i < text.size(); i++){
        /*std::cout << "(size+spacing) * 1) + (-0.5f * size)" << (((size+spacing) * 1.0f)+ (-0.5f * size)) << std::endl;
        std::cout << "0.5f * size" << (0.5f * size) << std::endl;
        std::cout << "-0.5f * size" << (-0.5f * size) << std::endl;*/
        
        int x = 2 + (text[i] - 'a') % 16;
        int y = 5 + (text[i] - 'a')/16;
        std::cout << "X: " << x << " Y: " << y << std::endl;
        
        Entity myEntity = *new Entity(0+(0.3*i),0,0.1,0.1);
        myEntity.sprite = SheetSprite(fontTexture, (spacing+(x*(spacing+size)))/512.0f, (spacing+(y*(spacing+size)))/512.0f, 20.0f/512.0f, 20.0f/512.0f, 0.2);
        myEntity.type = PLATFORM;
        myEntity.isStatic = true;
        menuEntities.push_back(myEntity);

    }
}

//solution: build custom drawtext
/*
void DrawText(ShaderProgram* program, int fontTexture, std::string text, float size, float spacing) {
    float texture_size = 1.0f/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
                std::cout << text[i] << " : " << texture_x << " # " << texture_y << std::endl;
        

        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        
        for(int j=0; j < vertexData.size(); j++) {
            std::cout << " | " << vertexData[j];
        }
        std::cout << std::endl;
        
        std::cout << "(size+spacing) * 1) + (-0.5f * size)" << (((size+spacing) * 1.0f)+ (-0.5f * size)) << std::endl;
        std::cout << "0.5f * size" << (0.5f * size) << std::endl;
        std::cout << "-0.5f * size" << (-0.5f * size) << std::endl;

        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}*/



void procedurallyGenerate(){
    for(int y = LEVEL_HEIGHT-2; y > 2; y--){
        for(int x = 2; x < LEVEL_WIDTH-2; x++){
            //if the tile 2 below you and 1 over is 1, 66% chance to be 1
            if (levelData[y+2][x+1] || levelData[y+2][x-1]) {
                if (rand() % 3){
                    levelData[y][x] = 1;
                }
            }
        }
    }
    for (int i = 0; i < LEVEL_HEIGHT; ++i){
        for (int j = 0; j < LEVEL_WIDTH; ++j){
            std::cout << levelData[i][j] << ' ';
        }
        std::cout << std::endl;
    }

}

Chunk createMapChunk(Chunk surroundingChunks[]){
    /*
     The rules for generating a chunk are such:
     Pass 1: If a (cardinally) adjacent chunk has a space the chunk has a bordering space
     Pass 2: If there is a cell with two adjacent spaces, it also becomes a space to "connect" them
     Pass 3: Cells that are spaces surrounded by brick convert a cardinal brick into a space, recursively n times
     Pass 4: If two spaces are diagonal to each other, then one of the other diagonals becomes breakable
     Pass 5: Sprinke breakable bricks everywhere. Maybe only near spaces
     */
//    20*10 at present sizes
    
    Chunk* ch = new Chunk();
    ch->print();
    
    //pass 1
    if (surroundingChunks != NULL) {
        if (surroundingChunks[1].size() > 0) {//check top border
            for (int i = 0; i < ch->width; i++) {
                if (1 == surroundingChunks[1].get(ch->height-1, i))
                    ch->set(0, i, 1);//where it has a hole, we make a hole
            }
        }
        if (surroundingChunks[3].size() > 0) {//check left border
            for (int i = 0; i < ch->height; i++) {
                if (1 == surroundingChunks[3].get(i, ch->width-1))
                    ch->set(i, 0, 1);//where it has a hole, we make a hole
            }
        }//4 is us
        if (surroundingChunks[5].size() > 0) {//check right border
            for (int i = 0; i < ch->width; i++) {
                if (1 == surroundingChunks[5].get(0, i))
                    ch->set(ch->height-i, i, 1);//where it has a hole, we make a hole
            }
        }
        if (surroundingChunks[7].size() > 0) {//check bottom border
            for (int i = 0; i < ch->height; i++) {
                if (1 == surroundingChunks[7].get(i, 0))
                    ch->set(i, ch->width-1, 1);//where it has a hole, we make a hole
            }
        }
    }
    else{//we should randomly seed or something
        for (int i = 0; i < ch->height; i++) {
            for (int j = 0; j < ch->width; j++) {
                if ((rand() % 100 < 20)){
                    ch->set(i, j, 1);
                }
            }
        }
    }

    ch->print();
    
    //pass 2
    for (int i = 0; i < ch->height; i++) {
        for (int j = 0; j < ch->width; j++) {
            int sum = ch->sumCardNeighbors(i, j);
            if (sum > 1){
                ch->set(i, j, 1);
            }
            else {
                ;//we turn a random one into a space
            }
        }
    }
    //TODO: remove later
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            levelData[i][j] = ch->get(i, j);
        }
    }

    
    return *ch;
}

void initEntityArray(GLuint spriteSheetTexture){
    
    Entity player = *new Entity(0,-0.5,0.13,0.13);
    player.sprite = SheetSprite(spriteSheetTexture, 792.0f/914.0f, 828.0f/936.0f, 48.0f/914.0f, 106.0f/936.0f, 0.2);
    player.type = PLAYER;
    player.isAlive = false;
    entities.push_back(player);
    
    //-1.5 to 1.5
    //-1.0 to 0.8
    
    for(int y=LEVEL_HEIGHT-1; y > 0 ; y--){
        for(int x=0; x < LEVEL_WIDTH; x++){
            if(levelData[y][x]){
                //(LEVEL_WIDTH*TILE_SIZE)
                Entity myEntity = *new Entity(-1.5+(x*TILE_X),-(-1.0+0.05+(y*TILE_Y)),TILE_X,TILE_Y) ;
                if (rand() % 100 < 20){
                    myEntity.type = BPLATFORM;
                    //<SubTexture name="boxEmpty.png" x="0" y="432" width="70" height="70"/>
                    myEntity.sprite = SheetSprite(spriteSheetTexture, 0.0f/914.0f, 432.0f/936.0f, 70.0f/914.0f, 70.0f/936.0f, 0.2);

                }
                else{
                    myEntity.type = PLATFORM;
                    //<SubTexture name="dirtCenter.png" x="720" y="864" width="70" height="70"/>
                    myEntity.sprite = SheetSprite(spriteSheetTexture, 504.0f/914.0f, 288.0f/936.0f, 70.0f/914.0f, 70.0f/936.0f, 0.2);
                }

                myEntity.isStatic = true;
                myEntity.isAlive = false;
                entities.push_back(myEntity);
            }
        }
    }
}

int countLiveBullets(){
    int tot = 0;
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].isAlive) {
            tot++;
        }
    }
    return tot;
}


//TODO: sometimes these are swallowed somehow
void shootBullet(int direction) {//N.E.W.S.
//    Mix_PlayChannel( -1, pewpew, 0);
    
    GLuint sSTex = LoadTexture(RESOURCE_FOLDER"tiles_spritesheet.png");//this is global
    Entity newBullet = *new Entity(PLAYER_ENT.xmid()-0.015,PLAYER_ENT.ymid()-0.015,0.03,0.03);
    newBullet.sprite = SheetSprite(sSTex, 0.0f/1024.0f, 576.0f/1024.0f, 70.0f/1024.0f, 70.0f/1024.0f, 0.2);
    newBullet.type = BULLET;
    switch (direction){
        case 0:
            newBullet.velocity_y = 2.0f;
            puts("up");
            break;
        case 1:
            newBullet.velocity_x = 2.0f;
            puts("right");
            break;
        case 2:
            newBullet.velocity_x = -2.0f;
            puts("left");
            break;
        case 3:
            newBullet.y -= 0.20;
            newBullet.velocity_y = -2.0f;
            puts("down");
            break;
    }
    bullets.push_back(newBullet);
    std::cout <<  newBullet.x << " : " << newBullet.y << std::endl;
}

void setupMainMenu(ShaderProgram* program, GLuint spriteSheetTexture){
    //create a menu UI and such
    
    GLuint textTexture = LoadTexture(RESOURCE_FOLDER"font1.png");
    
    state = STATE_MAIN_MENU;

    //TODO this is all random
    float size = 12.5f;
    float spacing = 12.1f;
    std::string text = "test";
    
    DrawText(program, textTexture,  text, size, spacing);//textTexture

}

void startGame(){
    for (int i = 0; i < menuEntities.size(); i++) {
        menuEntities[i].isAlive = false;
    }
    for (int i = 0; i < entities.size(); i++) {
        entities[i].isAlive = true;
    }
}

ShaderProgram Setup(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platforms", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    projectionMatrix.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glViewport(0, 0, 800, 600);
    
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(0.46f, 0.81f, 1.0f, 1.0f);
    GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"tiles_spritesheet.png");
    std::cout << "ssTex:" << spriteSheetTexture << std::endl;
    setupMainMenu(&program, spriteSheetTexture);
    initEntityArray(spriteSheetTexture);

    
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    return program;
}

void PlayerInput(bool& done){
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        
        else if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                if (PLAYER_ENT.leftContact) {
                    puts("not lefting");//do nothing
                }
                else{
                    PLAYER_ENT.acceleration_x = -1.2;
                    if (PLAYER_ENT.rightContact) {
                        PLAYER_ENT.rightContact = false;
                        //PLAYER_ENT.acceleration_x = -6.0;
                    }
                }
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                if (PLAYER_ENT.rightContact) {
                    puts("not righting");//do nothing
                }
                else{
                    PLAYER_ENT.acceleration_x = 1.2;
                    if (PLAYER_ENT.leftContact) {
                        PLAYER_ENT.leftContact = false;
                        //PLAYER_ENT.acceleration_x = 6.0;
                    }
                }
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                if (state == STATE_MAIN_MENU) {
                    state = STATE_GAME_LEVEL;
                    startGame();
                }
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_W){
                shootBullet(0);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_D){
                shootBullet(1);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_A){
                shootBullet(2);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_S){
                shootBullet(3);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_J){//jetpack TODO: remove later
                if (state != STATE_MAIN_MENU) {
                        PLAYER_ENT.velocity_y += 3.0f;
                }
            }

            else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                if (PLAYER_ENT.bottomContact) {
                    PLAYER_ENT.velocity_y += 3.0f;
                    PLAYER_ENT.bottomContact = false;
                }
                else if (PLAYER_ENT.leftContact) {
                    PLAYER_ENT.velocity_y += 3.0f;
                    PLAYER_ENT.velocity_x += 1.7f;
                    PLAYER_ENT.leftContact = false;
                    PLAYER_ENT.rightContact = false;
                }
                else if (PLAYER_ENT.rightContact) {
                    PLAYER_ENT.velocity_y += 3.0f;
                    PLAYER_ENT.velocity_x -= 1.7f;
                    PLAYER_ENT.rightContact = false;
                    PLAYER_ENT.leftContact = false;
                }
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                done = true;
            }
        }
        else if (event.type == SDL_KEYUP){
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                PLAYER_ENT.acceleration_x = 0;
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                PLAYER_ENT.acceleration_x = 0;
            }
            
        }

    }
}

void Update(float elapsed, ShaderProgram* program){
    if(state == STATE_MAIN_MENU) {
        for(int i=0; i < menuEntities.size(); i++) {//loop through entities and update all
            menuEntities[i].Update(elapsed);
        }
    }
    else if (state == STATE_GAME_LEVEL){
        for(int i=0; i < entities.size(); i++){//loop through entities and update all
            entities[i].Update(elapsed);
        }
        
        for(int i=0; i < entities.size(); i++) {
            if (entities[i].isAlive && !entities[i].isStatic){
                for(int j=0; j < entities.size(); j++){
                    if(i!=j && entities[j].isAlive && entities[i].collidesWith(entities[j])){
                        entities[i].uncollide(entities[j]);
                    }
                }
            }
        }
        //bullet stuff
        for(int i=0; i < bullets.size(); i++){
            if (bullets[i].isAlive){
                bullets[i].Update(elapsed);
                for (int j = 0; j < entities.size(); j++){
                    if(j != 0 && entities[j].isAlive && bullets[i].collidesWith(entities[j])){//do collision stuffs
                        if (entities[j].type == BPLATFORM){
                            entities[j].isAlive = false;
                            entities[j].~Entity();
                        }
                        bullets[i].isAlive = false;
                        bullets[i].~Entity();
                    }
                }
            }
        }
        viewMatrix.identity();
        viewMatrix.Translate(-(PLAYER_ENT.x), -(PLAYER_ENT.y), 0);//(PLAYER_ENT.y)
        program->setViewMatrix(viewMatrix);
    }
}

void fixedTSUpdate(ShaderProgram* program){
    //take care of ticks here
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + timeLeftOver;
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP){
        fixedElapsed -= FIXED_TIMESTEP;
        Update(FIXED_TIMESTEP, program);
    }
    timeLeftOver = fixedElapsed;
}

void Render(ShaderProgram* program){
    glClear(GL_COLOR_BUFFER_BIT);
    if (state == STATE_MAIN_MENU) {
        for(int i=0; i < entities.size(); i++){
            menuEntities[i].Render(program);
        }
    }
    else if (state == STATE_GAME_LEVEL) {
        for(int i=0; i < entities.size(); i++){
            entities[i].Render(program);
        }
        for(int i=0; i < bullets.size(); i++){
            bullets[i].Render(program);
        }
    }

    SDL_GL_SwapWindow(displayWindow);
}

void setupSound(){
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    pewpew = Mix_LoadWAV(RESOURCE_FOLDER"Laser.wav");
    
    music = Mix_LoadMUS( "Clockwork_Grey_Interlude.mp3");
    Mix_PlayMusic(music, -1);
}

int main(int argc, char *argv[]){
    state = STATE_GAME_LEVEL;
    createMapChunk(NULL);//TODO: use this properly
    procedurallyGenerate();
    
    ShaderProgram program = Setup();
    //setupSound();
    
    done = false;
    while (!done) {
        PlayerInput(done);
        fixedTSUpdate(&program);
        Render(&program);
        glUseProgram(program.programID);
    }
    for (int i = 0; i < entities.size(); i++) {
        entities[i].~Entity();
    }
    for (int i = 0; i < menuEntities.size(); i++) {
        menuEntities[i].~Entity();
    }
    //Mix_FreeChunk(someSound);
    //Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}