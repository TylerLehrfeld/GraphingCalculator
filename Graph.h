#include <SDL2/SDL.h>
#include "Equation.h"
#include <string>

using namespace std;

class Graph {
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Equation equation;

    public:
    void init();

    void graphLine(string equationString);
};

