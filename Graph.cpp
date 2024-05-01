#include "Graph.h"

void Graph::init()
{
    SDL_Window *window = NULL;
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
        640 * 2,
        480 * 2,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        printf("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 0, 20, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

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

void Graph::graphLine(string equationString) {
    equation.translate(equationString);
}