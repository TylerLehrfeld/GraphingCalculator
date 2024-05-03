#include "Graph.h"
#include <iostream>

using namespace std;

Graph::~Graph()
{
    cout << "destroying graph" << endl;
};

Graph::Graph()
{
    pixels = nullptr;
    pitch = 0;
}

void Graph::init()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(
        "SDL window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        printf("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    //draws the axis lines
    drawAxis();

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                string inputEquation;
                cout << "Enter your equation here" << endl;
                cin >> inputEquation;
                graphLine(inputEquation);
            }
        }
    }

    cout << "destroying assets" << endl;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graph::graphLine(string equationString)
{
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
    {
        cout << "error in graph line: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    bool lastHeightInitialized = false;
    int lastHeight = 0;
    for (int i = 0; i < WINDOW_WIDTH; i++)
    {
        Equation *equation = new Equation();
        equation->translate(equationString);
        vector<float> variable_values{(float)((float)(-WINDOW_WIDTH / 2 + i) / (float)WINDOW_WIDTH * (float)XRANGE)};
        float result = (equation->evaluate(variable_values));
        result = result/(float)YRANGE * (float)(WINDOW_HEIGHT);
        
        result += WINDOW_HEIGHT / 2;
        if (result < WINDOW_HEIGHT && result > 0)
        {
            if (!lastHeightInitialized)
            {
                lastHeight = result;
                lastHeightInitialized = true;
                Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)result) * (pitch / sizeof(Uint32));
                *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
            }
            else
            {
                if (lastHeight <= result)
                {
                    for (int j = lastHeight; j <= result; j++)
                    {
                        Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)j) * (pitch / sizeof(Uint32));
                        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
                    }
                }
                else
                {
                    for (int j = result; j <= lastHeight; j++)
                    {
                        Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)j) * (pitch / sizeof(Uint32));
                        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
                    }
                }
                lastHeight = result;
            }
        }
        variable_values.clear();
        delete equation;
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Graph::drawAxis()
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
    {
        cout << "error in draw axis: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    for (int i = 0; i < WINDOW_HEIGHT; i++)
    {
        Uint32 *pixel = static_cast<Uint32 *>(pixels) + WINDOW_WIDTH / 2 + i * (pitch / sizeof(Uint32));
        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
    }
    for (int i = 0; i < WINDOW_WIDTH; i++)
    {
        Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + WINDOW_HEIGHT / 2 * (pitch / sizeof(Uint32));
        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
    }
    SDL_UnlockTexture(texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

// 0,0 is top left.
// WINDOW_WIDTH-1,WINDOW_HEIGHT-1 is bottom right
void Graph::drawPixel(int x, int y)
{
    if (x < 0 || x >= WINDOW_WIDTH)
    {
        cout << "width out of bounds: " << x << (x > 0 ? " is greater than " : " is less than ") << (x > 0 ? WINDOW_WIDTH - 1 : 0) << endl;
        return;
    }
    if (y < 0 || y >= WINDOW_HEIGHT)
    {
        cout << "height out of bounds: " << y << (y > 0 ? " is greater than " : " is less than ") << (y > 0 ? WINDOW_HEIGHT - 1 : 0) << endl;
        return;
    }
    Uint32 *pixel = static_cast<Uint32 *>(pixels) + x + y * (pitch / sizeof(Uint32));
    *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
}