#include "App.h"

App::App() {

	SDL_Init(SDL_INIT_VIDEO); //Init SDL
    IMG_Init(IMG_INIT_PNG);

	SDL_GetCurrentDisplayMode(0, &DM); //Get DisplayMode for Primary Monitor.


    //Due to a weird limitation with creating the window always on top in SDL, we have to wait until their cursor is on their main monitor.
    POINT p;
    while (true) {
        GetCursorPos(&p);
        if (0 <= p.x && p.x <= DM.w && 0 < p.y && p.y <= DM.h)
            break;
        Sleep(100);
    }


    shouldRun = true;
    fps = 60.0f;

	//Create Window
	Uint32 flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP;
	SDL_CreateWindowAndRenderer(DM.w, DM.h, flags, &window, &renderer);


	SDL_VERSION(&info.version); //Get info about platform.


    needsGraphicsFlush = true;


    customizeWindow();

    //Particle Engine Stuff.
    particleEngine.init(renderer);

    //Testing by spawning particles.
    //for (int i = 0; i < 0; i++) {
        //SDL_Point p;
        //p.x = rand() % 1920;
        //p.y = rand() % 1080;
        //particleEngine.spawnParticle(p);
    //}

    truePoints = 5000;
    
}

void App::render()
{

    if (particleEngine.needsRendering()) {
        //Set background to black (transparent).
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);


        particleEngine.render(renderer);


        //Present rendering to screen.
        SDL_RenderPresent(renderer);

        needsGraphicsFlush = true;
    }
    else if (needsGraphicsFlush) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        needsGraphicsFlush = false;
    }
}

void App::update() {
    /*
    SDL_Color color;
    color.r = 0xFF;
    color.b = 0x00;
    color.g = 0x00;
    color.a = 0xFF;
    for (int i = 0; i < 1; i++) {
        SDL_Point p;
        p.x = rand() % 1920;
        p.y = rand() % 1080;
        particleEngine.spawnParticle(p, color);
    }
    */
    particleEngine.update(lastTimeDifference, &DM);

    mouseEngine.addPoints(particleEngine.getDeleteCount()); //Extract count of deleted particles from engine and add those to mouse points.

    mouseEngine.update();


    //Spawn points if we're short.
   

    if ((truePoints - (mouseEngine.getPoints() + particleEngine.getPointParticleCount())) >= 4) {
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                SDL_Point velocity, point;

                static const int SHOOTSPEED = 100;

                if (x == 0)
                    velocity.x = SHOOTSPEED;
                else
                    velocity.x = -1 * SHOOTSPEED;
                if (y == 0)
                    velocity.y = SHOOTSPEED;
                else
                    velocity.y = -1 * SHOOTSPEED;
                if (x == 0)
                    point.x = 0;
                else
                    point.x = DM.w - 1;
                if (y == 0)
                    point.y = 0;
                else
                    point.y = DM.h - 1;
                particleEngine.spawnParticle(point, velocity);
            }
        }
    }

}

void App::mainLoop() {
    //Frame limit stuff.

    while (shouldRun) {
        //Start Clocks.
        fpsControlClock = std::chrono::system_clock::now();
        fpsClock = std::chrono::system_clock::now();


        //Bulk Function Loops.
        update();
        render();
        
        SDL_Event pollEvent;
        SDL_PollEvent(&pollEvent);


        customizeWindow(); //What if I just call this every frame?

        //Wait if we exceed the our goal FPS.
        const std::chrono::duration<double> controlDuration = std::chrono::system_clock::now() - fpsControlClock;
        const double frameDifference = (1.0 / fps) - controlDuration.count();
        if (frameDifference > 0.0) //If we are above target FPS. If we are below, skip this.
            std::this_thread::sleep_for(std::chrono::milliseconds(int(frameDifference * 1000)));

        //Find actual FPS.
        const std::chrono::duration<double> actualDuration = std::chrono::system_clock::now() - fpsControlClock;

        std::cout << "FPS: " << 1.0 / actualDuration.count() << std::endl;

        lastTimeDifference = actualDuration;

    }


    cleanup();
    return;
}

void App::cleanup() {
    //Cleanup.
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_Quit();
    return;
}

void App::customizeWindow() {
    //Customize window using winapi.
    if (SDL_GetWindowWMInfo(window, &info)) { /* the call returns true on success */
        HWND hwnd = info.info.win.window; //Get window handle from info object.
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST); //Set all the options to customize window.
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); //Make it so that straight black color renders as a transparent window.
    }
    else {
        /* call failed */
        printf("Couldn't initialize window properly. Closing.");
        close();
    }
}
