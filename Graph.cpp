#include "Graph.h"
#include <iostream>

using namespace std;

void Graph::init()
{
    window = NULL;
    // SDL_Surface* screenSurface = NULL;
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
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graph::graphLine(string equationString)
{
    equation.translate(equationString);
    vector<float> variable_values{1.002};
    float result = equation.evaluate(variable_values);
    cout << " Final value: " << result << endl;
}

void Graph::drawAxis()
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    void *pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
    {
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