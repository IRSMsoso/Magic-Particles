// Magic Particles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "App.h"

int main(int argc, char* argv[]) {
    
    //ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    App app;
    app.setFPS(120);
    app.mainLoop();
    //MouseEngine mouseEngine;

    //system("pause");
    return 0;
}
