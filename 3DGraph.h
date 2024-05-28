#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "Equation.h"
#include "NewEquationParser.h"
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
    Point projectToViewPlane(double x, double y, double z);
    void drawPoint(double x, double y, int r, int g, int b);
    
    ~ThreeDGraph();
    ThreeDGraph();


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    void zoom(double zoomDiff);
    void* threadFunc1(void* args);
    void rotate(double xDegrees, double zDegrees);
    Point Normal;
    void setBounds();
    void initializeSDLVariables();
    void beginGameLoop();
    //void drawX();
    void drawAxis();
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
    string _equationString;
};


#endif