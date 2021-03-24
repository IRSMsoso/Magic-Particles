#include "App.h"

App::App(): particleEngine(&DM) {

	SDL_Init(SDL_INIT_VIDEO); //Init SDL
    IMG_Init(IMG_INIT_PNG);

	SDL_GetCurrentDisplayMode(0, &DM); //Get DisplayMode for Primary Monitor.

    shouldRun = true;


    //Instantiate ParticleEngine.


    //Get the user_id from file
    std::fstream file;
    
    file.open("discord_id.txt", std::ios::in);
    if (file) {
        std::string id(std::istreambuf_iterator<char>{file}, {});
        discordID = id;
    }
    else {
        file.open("discord_id.txt", std::ios::out); //Create file if not created.
        file.close();
        shouldRun = false;
    }



    //Due to a weird limitation with creating the window always on top in SDL, we have to wait until their cursor is on their main monitor.
    POINT p;
    while (true) {
        GetCursorPos(&p);
        if (0 <= p.x && p.x <= DM.w && 0 < p.y && p.y <= DM.h)
            break;
        Sleep(100);
    }


    fps = 60.0f;

	//Create Window
	Uint32 flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP;
	SDL_CreateWindowAndRenderer(DM.w, DM.h, flags, &window, &renderer);


	SDL_VERSION(&info.version); //Get info about platform.


    needsGraphicsFlush = true;
    shouldCatchup = true;

    setWindowVeiled(true);



    //Networking init stuff.
    socket.setBlocking(false);
    networkClock = std::chrono::system_clock::now();

    //Overlay init stuff.
    isOverlayShown = false;
    transitionHeight = 0;
    screenCaptureTexture = nullptr;
    screenCaptureSurface = nullptr;
    overlayCooldown = std::chrono::system_clock::now();


    //Particle Engine Stuff.
    particleEngine.init(renderer);

    truePoints = 0;
    
}

//Called from mainLoop(). Renders the screen. Only does so if we have objects on the screen that need rendering. Called every frame.
void App::render()
{

    if (shouldRender()) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        //Overlay Rendering

        
        //Set background to black (transparent).


        particleEngine.render(renderer);


        //Present rendering to screen.
        SDL_RenderPresent(renderer);

        needsGraphicsFlush = true;
        

        SDL_RenderPresent(renderer);

    }
    else if (needsGraphicsFlush) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        needsGraphicsFlush = false;
    }
}

//Called from mainLoop(). This calls all the "update" related functions, including those of member objects. Called every frame.
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


    // Spawning particles of we're short and all that.
    appLogic();


    //Networking stuff.
    updateNetworking();


    //Overlay Update stuff
    

}

//The main loop that we will be stuck in for a majority of the program's life.
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

        while (SDL_PollEvent(&pollEvent)) { //This is needed to keep window from going unresponsive. Internally (as I understand), SDL uses this function to communicate with windows and stay alive.

        }

        //customizeWindow(); //What if I just call this every frame? NO! not needed.

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

//Cleansup windows and such from SDL. Called when app finishes running.
void App::cleanup() {
    //Cleanup.
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_Quit();
    return;
}

BOOL SaveBitmap(HDC hDC, HBITMAP hBitmap, std::string fileDestination)
{
    OutputDebugString(L"Start SaveBitmap() ");
    DWORD error;
    FILE* fp = NULL;
    fp = fopen(fileDestination.c_str(), "wb");
    if (fp == NULL)
    {
        OutputDebugString(L"Error Unable to Create File ");
        return false;
    }
    BITMAP Bm;
    BITMAPINFO BitInfo;
    ZeroMemory(&BitInfo, sizeof(BITMAPINFO));
    BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitInfo.bmiHeader.biBitCount = 0;

    if (!::GetDIBits(hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS))
    {
        OutputDebugString(L"Error GetDIBits Fail");

        return (false);
    }
    Bm.bmHeight = BitInfo.bmiHeader.biHeight;
    Bm.bmWidth = BitInfo.bmiHeader.biWidth;

    BITMAPFILEHEADER    BmHdr;

    BmHdr.bfType = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
    BmHdr.bfSize = (((3 * Bm.bmWidth + 3) & ~3) * Bm.bmHeight)
        + sizeof(BITMAPFILEHEADER)
        + sizeof(BITMAPINFOHEADER);
    BmHdr.bfReserved1 = BmHdr.bfReserved2 = 0;
    BmHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)
        + sizeof(BITMAPINFOHEADER);

    BitInfo.bmiHeader.biCompression = 0;
    // Writing Bitmap File Header ////
    size_t size = fwrite(&BmHdr, sizeof(BITMAPFILEHEADER), 1, fp);
    if (size < 1)
    {
        OutputDebugString(L"Error  Header Write");
        error = GetLastError();
    }
    size = fwrite(&BitInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    if (size < 1)
    {
        OutputDebugString(L"Error  Write");
        error = GetLastError();
    }
    BYTE* pData = new BYTE[BitInfo.bmiHeader.biSizeImage + 5];
    if (!::GetDIBits(hDC, hBitmap, 0, Bm.bmHeight,
        pData, &BitInfo, DIB_RGB_COLORS))
        return (false);
    if (pData != NULL)
        fwrite(pData, 1, BitInfo.bmiHeader.biSizeImage, fp);

    fclose(fp);
    delete (pData);

    return (true);
}

void App::toggleOverlay() {
    if (std::chrono::system_clock::now() - overlayCooldown > std::chrono::seconds(2)) { //Implements a cooldown period for switching the overlay.
        isOverlayShown = !isOverlayShown;


        std::cout << "Overlay Activated\n";


        particleEngine.clearParticlesOfType(ParticleType::PixelParticleType);

        CaptureScreen();
        //setWindowVeiled(false);
        //transitionHeight = 0;
        const int DIVISIONSIZE = 15; //Common Denominators available between 1080 and 1920 include 2,3,4,5,6,8,10,12,15,20,24


        int bpp = screenCaptureSurface->format->BytesPerPixel;

        std::cout << "Bytes per pixel: " << bpp << std::endl;



        for (int y = 0; y < DM.h; y += DIVISIONSIZE) {
            for (int x = 0; x < DM.w; x += DIVISIONSIZE) {

                Uint32* color = new Uint32;

                int red = 0, green = 0, blue = 0;

                for (int dy = 0; dy < DIVISIONSIZE; dy++) {
                    for (int dx = 0; dx < DIVISIONSIZE; dx++) {
                        Uint8* p = (Uint8*)screenCaptureSurface->pixels + (y + dy) * screenCaptureSurface->pitch + (x + dx) * bpp;

                        red += (int)p[2];
                        green += (int)p[1];
                        blue += (int)p[0];

                    }
                }

                red /= (DIVISIONSIZE * DIVISIONSIZE);
                green /= (DIVISIONSIZE * DIVISIONSIZE);
                blue /= (DIVISIONSIZE * DIVISIONSIZE);

                //std::cout << "Average Red: " << red << std::endl;

                if (red == 0)
                    red = 1;
                if (green == 0)
                    green = 1;
                if (blue == 0)
                    blue = 1;

                //std::cout << "Average Red: " << red << std::endl;

                ((Uint8*)color)[2] = red;
                ((Uint8*)color)[1] = green;
                ((Uint8*)color)[0] = blue;


                //Test
                MathVector point(x + DIVISIONSIZE / 2.0, y + DIVISIONSIZE / 2.0);
                PixelParticle* newParticle = new PixelParticle(point, particleEngine.getMousePosition(), &DM, color, DIVISIONSIZE);
                particleEngine.spawnParticle(newParticle);
            }
            std::cout << y << std::endl;
        }

        render(); //Call to flush new info to the rendering target before we switch up the window settings.

        setWindowVeiled(!isOverlayShown);

        overlayCooldown = std::chrono::system_clock::now(); //Restart clock
    }
    
}

void App::CaptureScreen() {

    if (screenCaptureTexture != nullptr)
        SDL_DestroyTexture(screenCaptureTexture);
    if (screenCaptureSurface != nullptr)
        SDL_FreeSurface(screenCaptureSurface);

    HDC hDC = GetDC(NULL);
    HDC hBitmapDC = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, DM.w, DM.h);
    SelectObject(hBitmapDC, hBitmap);


    if (!BitBlt(hBitmapDC, 0, 0, DM.w, DM.h, hDC, 0, 0, SRCCOPY)) {
        std::cout << GetLastError() << std::endl;
    }
    else {
        SaveBitmap(hBitmapDC, hBitmap, "capture.bmp");
        screenCaptureTexture = IMG_LoadTexture(renderer, "capture.bmp");
        screenCaptureSurface = IMG_Load("capture.bmp");
        std::cout << IMG_GetError() << std::endl;
    }

    DeleteDC(hBitmapDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);

}

bool App::shouldRender() {

    QUERY_USER_NOTIFICATION_STATE pquns;
    SHQueryUserNotificationState(&pquns);
 
    SDL_GetWindowWMInfo(window, &info);
    bool preOccupied = ((isFullscreen(GetForegroundWindow()) && (GetForegroundWindow() != info.info.win.window)) || !cursorShown());

    //std::cout << "Foreground Window: " << GetForegroundWindow() 

    if (!preOccupied && particleEngine.hasParticlesToRender()) {
        return true;
    }
    else {
        return false;
    }
}

//Manages the network logic for the app. This includes connecting, sending, receiving, and setting appropriate data. Called every frame.
void App::updateNetworking() {

    sf::IpAddress sender;
    unsigned short port;
    std::size_t received;
    char* receiveData = new char[100]; //Allocate new data


    socket.setBlocking(false);
    sf::Socket::Status status = socket.receive(receiveData, 1024, received);
    if (status == sf::Socket::Done) {
        std::string message(receiveData, received);
        //std::cout << "Received: " << message << std::endl;
        //std::cout << "Size Received: " << received << std::endl;
        unsigned int newPointValue = std::stoi(message);
        if (shouldCatchup)
            mouseEngine.setPoints(newPointValue);  //This is used to bring a newly opened client up to speed on their points.
        shouldCatchup = false;

        truePoints = newPointValue;
    }
    else if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
        socket.setBlocking(true); //This needs to be non-blocking or it doesn't end up connecting long-distance where connections can be slow.
        std::cout << "Disconnected.. Connecting again\n";
        std::cout << "Connection Code: " << socket.connect(sf::IpAddress("173.24.79.165"), 54562) << std::endl;

    }
    else {
        //std::cout << "Status: " << status << std::endl;
    }

    socket.setBlocking(true);
    const std::chrono::duration<double> timeDiff = std::chrono::system_clock::now() - networkClock;
    if (timeDiff.count() > 5 && socket.getRemoteAddress() != sf::IpAddress::None) {
        //Prepare Data
        std::string sendString = discordID; //What to send.
        char* sendData = new char[sendString.length() + 1]; //Allocate new data
        std::strcpy(sendData, sendString.c_str());

        //Send Data
        std::cout << "Sending Data\n";
        std::cout << socket.send(sendData, sendString.length()) << std::endl;
        //std::cout << "Sent Data\n";
        networkClock = std::chrono::system_clock::now();
        delete[] sendData;
    }

    delete[] receiveData;

}

//Does logic for the app itself. This includes spawning particles atm. Called every frame.
void App::appLogic() {

    //Spawn point particles if we're short.
    if (((int)truePoints - ((int)mouseEngine.getPoints() + (int)particleEngine.getPointParticleCount())) >= 4) {

        //std::cout << "TruePoints (" << truePoints << ") - (MouseEngine (" << mouseEngine.getPoints() << ") + ParticleCount (" << particleEngine.getPointParticleCount() << ")) >= 4 is true\n";
        //std::cout << "Value: " << (truePoints - (mouseEngine.getPoints() + particleEngine.getPointParticleCount())) << std::endl;

        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                MathVector velocity, point;

                static const int SHOOTSPEED = 100;

                if (x == 0)
                    velocity.x = SHOOTSPEED;
                else
                    velocity.x = -1.0 * SHOOTSPEED;
                if (y == 0)
                    velocity.y = SHOOTSPEED;
                else
                    velocity.y = -1.0 * SHOOTSPEED;
                if (x == 0)
                    point.x = 0;
                else
                    point.x = DM.w - 1.0;
                if (y == 0)
                    point.y = 0;
                else
                    point.y = DM.h - 1.0;

                AddPointParticle* newParticle = new AddPointParticle(point, particleEngine.getMousePosition(), &DM);
                newParticle->setVelocity(velocity);
                particleEngine.spawnParticle(newParticle);
            }
        }
    }

    //Lose points if we're over.
    if (mouseEngine.getPoints() > truePoints){
        std::cout << "Mouse Engine: " << mouseEngine.getPoints() << std::endl;
        std::cout << "True Points: " << truePoints << std::endl;
        POINT p = *particleEngine.getMousePosition();
        MathVector position(p.x + 4.0, p.y + 9.0);
        LosePointParticle* newParticle = new LosePointParticle(position, particleEngine.getMousePosition());
        MathVector velocity;
        double radianAngle = ((rand() % 360) * PI / 180.0); //Picks random angle to shoot particle at.
        const double SPEEDMODIFIER = 200.0;
        double randScalar = ((double(rand() % 5) + 5.0) / 10.0) * SPEEDMODIFIER; //Will multiply speedmodifier by a random value between 0.5 and 0.9.
        velocity.x = randScalar * cos(radianAngle);
        velocity.y = randScalar * sin(radianAngle);
        newParticle->setVelocity(velocity);
        particleEngine.spawnParticle(newParticle);
        mouseEngine.removePoints(1);
    }


    //Overlay Logic
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {

        toggleOverlay();

    }

}


void App::setWindowVeiled(bool veil) {

    //Customize window using winapi.
    if (SDL_GetWindowWMInfo(window, &info)) { /* the call returns true on success */
        HWND hwnd = info.info.win.window; //Get window handle from info object.

        long style = GetWindowLong(hwnd, GWL_EXSTYLE);

        if (veil) {
            SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST); //Set all the options to customize window.
            SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); //Make it so that straight black color renders as a transparent window.
        }
        else {
            style &= ~WS_EX_LAYERED;
            style &= ~WS_EX_TRANSPARENT;
            SetWindowLong(hwnd, GWL_EXSTYLE, style); //Set all the options to customize window.
        }

    }
    else {
        /* call failed */
        printf("Couldn't initialize window properly. Closing.");
        close();
    }


}
