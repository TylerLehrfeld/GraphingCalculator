#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "Equation.h"
#include "Point.h"
#ifndef THREE_D_GRAPH
#define THREE_D_GRAPH

using namespace std;

class windowDefinition {
public:
    double minX;
    double maxX;
    double minY;
    double maxY;
    double WINDOW_WIDTH;
    double WINDOW_HEIGHT;
    Point toPixel(double x, double y) {
        double Xrange = maxX - minX;
        double xVal = x - minX;
        double Yrange = maxY - minY;
        double yVal = y - minY;

        return Point(xVal / Xrange * WINDOW_WIDTH, yVal / Yrange * WINDOW_HEIGHT, 0);
    };
};

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
    Point projectToViewPlane(double x, double y, double z);
    Point Normal;
    void setBounds();
    void initializeSDLVariables();
    void beginGameLoop();
    void drawPoint(double x, double y);
    void* pixels;
    int pitch;

    double XRANGE = 10;
    double YRANGE = 10;
    double ZRANGE = 10;
    double WINDOW_WIDTH = 1000;
    double WINDOW_HEIGHT = 1000;
    windowDefinition graphingWindow;
    double thetaX = M_PI / 4;
    double thetaZ = -M_PI / 4;
    vector<string> equationList;
};


#endif