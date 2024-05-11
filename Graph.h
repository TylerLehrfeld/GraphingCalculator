#include <SDL2/SDL.h>
#include "Equation.h"
#include <string>

using namespace std;


struct lineElement {
    string equationString;
    bool isCDF;
    double testStat;
    int testEnumValue;
};

class Graph {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int WINDOW_WIDTH = 1500;
    int WINDOW_HEIGHT = 1000;
    float XRANGE = 10;
    float YRANGE = 10;
    void* pixels;
    int pitch;
    void drawPixel(int x, int y);
    void tTest();
    void normal();
    void chiSquaredTest();
    void zoom(int);
    enum testEnum {
        LINE = 0,
        NORMAL = 1,
        T_TEST = 2,
        CHI_SQUARED = 3,
    };
    int k;

    int AlternativeHypNumber;
    vector<lineElement> equations;
public:
    void init();

    void graphLine(string equationString, bool, float, int);
    void drawAxis();

    
    ~Graph();
    Graph();
};


