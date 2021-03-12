#include "App.h"

App::App() {

	SDL_Init(SDL_INIT_VIDEO); //Init SDL
    IMG_Init(IMG_INIT_PNG);

	SDL_GetCurrentDisplayMode(0, &DM); //Get DisplayMode for Primary Monitor.

    shouldRun = true;
    fps = 60.0f;

	//Create Window
	Uint32 flags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS;
	SDL_CreateWindowAndRenderer(DM.w, DM.h, flags, &window, &renderer);


	SDL_VERSION(&info.version); //Get info about platform.


    //Customize window using winapi.
    if (SDL_GetWindowWMInfo(window, &info)) { /* the call returns true on success */
        HWND hwnd = info.info.win.window; //Get window handle from info object.
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW); //Set all the options to customize window.
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); //Make it so that straight black color renders as a transparent window.
    }
    else {
        /* call failed */
        printf("Couldn't initialize window properly. Closing.");
        close();
    }


    //Particle Engine Stuff.
    particleEngine.init(renderer);

    //Testing by spawning particles.
    SDL_Color color;
    color.r = 0xFF;
    color.b = 0x00;
    color.g = 0x00;
    color.a = 0xFF;
    for (int i = 0; i < 5000; i++) {
        SDL_Point p;
        p.x = rand() % 1920;
        p.y = rand() % 1080;
        particleEngine.spawnParticle(p, color);
    }
    
}

void App::render()
{
    //Set background to black (transparent).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);


    //Simple rectangle.
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

    //
    particleEngine.render(renderer);


    //Present rendering to screen.
    SDL_RenderPresent(renderer);
}

void App::update() {
    particleEngine.update(lastTimeDifference, &DM);
    //std::cout << lastTimeDifference.count() << std::endl;
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
