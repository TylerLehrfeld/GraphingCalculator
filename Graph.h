#include <SDL2/SDL.h>
#include "Equation.h"
#include <string>

using namespace std;

class Graph {
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int WINDOW_WIDTH = 1500;
    int WINDOW_HEIGHT = 1200;
    float XRANGE = 20;
    float YRANGE = 4;
    void* pixels;
    int pitch;
    void drawPixel(int x, int y);
    void tTest();
    void normal();

    public:
    void init();

    void graphLine(string equationString, bool, float);
    void drawAxis();

    
    ~Graph();
    Graph();
};
