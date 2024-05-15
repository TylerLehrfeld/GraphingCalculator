#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "Equation.h"
#ifndef THREE_D_GRAPH
#define THREE_D_GRAPH

using namespace std;

class ThreeDGraph {
public:
    void init();
    void addSurface(string equationString);
    ~ThreeDGraph();
    ThreeDGraph();


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    void zoom(double zoomDiff);
    void rotate(double xDegrees, double yDegrees, double zDegrees);
    Point projectToViewPlane(double x, double y, double z, Point&, Point&);
    void setBounds();
    void initializeSDLVariables();
    void beginGameLoop();
    double XRANGE = 10;
    double YRANGE = 10;
    double ZRANGE = 10;
    double WINDOW_WIDTH = 1000;
    double WINDOW_HEIGHT = 1000;
    double _zDegrees = 60;
    double _xDegrees = 315;
    vector<string> equationList;
    };

#endif