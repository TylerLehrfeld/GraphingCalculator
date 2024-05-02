#include <SDL2/SDL.h>
#include "Equation.h"
#include <string>

using namespace std;

class Graph {
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* texture;
    Equation equation;
    int WINDOW_WIDTH = 1000;
    int WINDOW_HEIGHT = 1000;

    public:
    void init();

    void graphLine(string equationString);
    void drawAxis();
};
