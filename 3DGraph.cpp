#include "3DGraph.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include "Point.h"



ThreeDGraph::ThreeDGraph() {}

ThreeDGraph::~ThreeDGraph() {
    equationList.clear();
}

void drawAxis() {

}

#include "GraphHelper.h"

void ThreeDGraph::initializeSDLVariables() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(
        "Click Graph to enter equation",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);


}

void ThreeDGraph::init() {
    initializeSDLVariables();

    projectToViewPlane(4, 6, 4);
    drawAxis();

    beginGameLoop();

    destroyGraph(texture, renderer, window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void ThreeDGraph::beginGameLoop() {
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } if (event.type == SDL_KEYDOWN) {
                char keyPressed = (char)(event.key.keysym.sym);
                if (keyPressed == 'l') {
                    string equationString;
                    cin >> equationString;
                    equationList.push_back(equationString);
                    for (string equation : equationList) {
                        addSurface(equation);
                    }
                }
            }
            SDL_Delay(20);
        }
    }
}

void ThreeDGraph::addSurface(string equationString) {
    double dx = (double)XRANGE * 2 / 1000;
    double dy = (double)YRANGE * 2 / 1000;

    //We want to create 10 contour X and Y contourlines for the range selected
    //so we choose 10 x points and evaluate all of the y points to get a contour line
    float result = 0;
    for (double x = -(double)XRANGE / 2; x <= (double)XRANGE / 2; x += 10 * dx) {
        for (double y = -(double)YRANGE / 2; y <= (double)YRANGE / 2; y += dy) {
            Equation eq;
            eq.translate(equationString);
            float result = eq.evaluate({ (float)x,(float)y });
            projectToViewPlane(x, y, result);
        }
    }

    //choose 10 y points and evaluate each at every x
    for (double y = -(double)YRANGE / 2; y <= (double)YRANGE / 2; y += 10 * dy) {
        for (double x = -(double)XRANGE / 2; x <= (double)XRANGE / 2; x += dx) {
            Equation eq;
            eq.translate(equationString);
            result = eq.evaluate({ (float)x,(float)y });
            projectToViewPlane(x, y, result);
        }
    }
}

void ThreeDGraph::projectToViewPlane(double x, double y, double z) {
    Point pointToProject(x, y, z);
    Point NormalVector(cos(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) / XRANGE, sin(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) / YRANGE, cos(_zDegrees / 360 * 2 * M_PI) / ZRANGE);
    Point planePoint(2 * cos(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) * XRANGE, 2 * sin(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) * YRANGE, 2 * cos(_zDegrees / 360 * 2 * M_PI) * ZRANGE);
    Point projectedPoint = pointToProject - (NormalVector * ((NormalVector * (pointToProject - planePoint)) * (1 / (NormalVector * NormalVector))));
    Point relativeProjectedPoint = projectedPoint - planePoint;
    Point AlignedPoint(2*XRANGE * cos(_xDegrees / 360 * 2 * M_PI) / sin(_zDegrees / 360 * 2 * M_PI), 2*YRANGE * sin(_xDegrees / 360 * 2 * M_PI) / sin(_zDegrees / 360 * 2 * M_PI), 0);
    AlignedPoint = AlignedPoint - planePoint;

    double angle = acos((AlignedPoint * relativeProjectedPoint)/((AlignedPoint * AlignedPoint)*(relativeProjectedPoint * relativeProjectedPoint)));
    double magnitude = relativeProjectedPoint * relativeProjectedPoint;
}
void zoom() {}

void rotate() {

}


