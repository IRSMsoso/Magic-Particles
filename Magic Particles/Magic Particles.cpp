// Magic Particles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "App.h"

int main(int argc, char* argv[]) {
    
    //ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    //App app;
    //app.setFPS(120);
    //app.mainLoop();
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 100, 100, 100, 100, SDL_WINDOW_BORDERLESS), -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
    MouseEngine mouseEngine;

    system("pause");
    return 0;
}
