#include <SDL2/SDL.h>
#include "Equation.h"
#include <string>

using namespace std;

class Graph {
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int WINDOW_WIDTH = 600;
    int WINDOW_HEIGHT = 500;
    float XRANGE = 10;
    float YRANGE = 20;
    void* pixels;
    int pitch;
    void drawPixel(int x, int y);

    public:
    void init();

    void graphLine(string equationString);
    void drawAxis();

    
    ~Graph();
    Graph();
};
