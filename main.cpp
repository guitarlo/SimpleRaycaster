// A Simple Raycaster
// Written in C++ with SDL2
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

// Fensterabmessungen
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// SDL-Variablen
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Spielerposition und Blickwinkel
float playerX = 10.0f;
float playerY = 10.0f;
float playerAngle = 0.0f;

// Karte (0: leerer Raum, 1: Wand)
const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 12;
int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,2,2,0,0,0,0,1},
    {1,0,0,0,0,3,3,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
};

// Initialisiert SDL und erstellt das Fenster und Renderer
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Simple Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Zeichnet die Szene
void drawScene() {
    int FOV=M_PI /3.0
    ;
    int col=0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_Rect Celing={0,0,WINDOW_WIDTH,WINDOW_HEIGHT/2};
    SDL_RenderFillRect(renderer,&Celing);

    SDL_SetRenderDrawColor(renderer, 80, 0, 0, 255);
    SDL_Rect Ground={0,WINDOW_HEIGHT/2,WINDOW_WIDTH,WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer,&Ground);
    // Durchlaufe die vertikalen Bildschirmspalten
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        // Berechne den Winkel des aktuellen Strahls
        float rayAngle = (playerAngle - FOV / 2.0f) + ((float)x / WINDOW_WIDTH) * FOV;

        float distanceToWall = 0.0f;
        bool hitWall = false;
        bool boundary = false;
        float wallX=0.0f;
        // Bewege den Strahl, bis eine Wand getroffen wird
        while (!hitWall && distanceToWall < 20.0f) {
            distanceToWall += 0.1f;

            int testX = (int)(playerX + distanceToWall * std::cos(rayAngle));
            int testY = (int)(playerY + distanceToWall * std::sin(rayAngle));
            int mapX=static_cast<int>(testX);
            int mapY=static_cast<int>(testY);
            if (testX < 0 || testX >= MAP_WIDTH || testY < 0 || testY >= MAP_HEIGHT) {
                hitWall = true; // Außerhalb der Karte
            } else {
                if (map[testX][testY] > 0) {
                    hitWall = true;
                    col=map[testX][testY];
                    // Überprüfe, ob der Strahl eine Wandkante trifft
                    float wallDistance = std::sqrt((testX - playerX) * (testX - playerX) + (testY - playerY) * (testY - playerY));
                    boundary = (wallDistance < 0.5f);
                    wallX=testX-mapX;
                }
            }
        }

        // Berechne die Wandhöhe auf dem Bildschirm
        int wallHeight = WINDOW_HEIGHT / distanceToWall;
        Uint8 shade=0;
        if (wallX> 0.5f)
            shade/=2;
        // Zeichne die Wand
        if (col==1){
            shade= static_cast<Uint8>(255-distanceToWall*15);
            SDL_SetRenderDrawColor(renderer, shade, shade, shade, 255);}
        if (col==2){
            shade= static_cast<Uint8>(255-distanceToWall*15);
            SDL_SetRenderDrawColor(renderer, shade, 0, 0, 255);}
        if (col==3){
            shade= static_cast<Uint8>(255-distanceToWall*15);
            SDL_SetRenderDrawColor(renderer, 0, shade, 0, 255);}
        SDL_RenderDrawLine(renderer, x, (WINDOW_HEIGHT - wallHeight) / 2, x, (WINDOW_HEIGHT + wallHeight) / 2);

        if (boundary) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT / 2);
        }
    }
    std::cout << "PLayerX: "<<playerX<<" PlayerY: "<<playerY<<std::endl;
    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    if (!initSDL()) {
        return 1;
    }

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Tastatursteuerung
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]) playerAngle -= 0.05f;
        if (state[SDL_SCANCODE_RIGHT]) playerAngle += 0.05f;
        if (state[SDL_SCANCODE_UP]) {
            playerX += 0.05f * std::cos(playerAngle);
            playerY += 0.05f * std::sin(playerAngle);
            if (map[(int)playerX,(int)playerY]>0){
                playerX += 0.05f * std::cos(playerAngle);
                playerY += 0.05f * std::sin(playerAngle);}

        }
        if (state[SDL_SCANCODE_DOWN]) {
            playerX -= 0.02f * std::cos(playerAngle);
            playerY -= 0.02f * std::sin(playerAngle);
        }

        // Zeichne die Szene
        drawScene();

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
