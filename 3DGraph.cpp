#include "3DGraph.h"
#include <iostream>
#include <sstream>
#include <math.h>



ThreeDGraph::ThreeDGraph() {
    pixels = nullptr;
    pitch = 0;
}

ThreeDGraph::~ThreeDGraph() {
    equationList.clear();
}

void ThreeDGraph::drawPoint(double x, double y) {
    Point pixelPoint = graphingWindow.toPixel(x, y);
    x = pixelPoint._x;
    y = pixelPoint._y;
    if(x <= graphingWindow.minX || x >= graphingWindow.maxX || y <= graphingWindow.minY || y >= graphingWindow.maxY) {
        return;
    } else {
        
        Uint32 *pixel = static_cast<Uint32 *>(pixels) + (int)(pixelPoint._x) + (int)(WINDOW_HEIGHT - pixelPoint._y) * (pitch / sizeof(Uint32));
        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
    
    }
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
    setBounds();
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

    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
    {
        cout << "error in graph line: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    Point NormalVector(cos(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) / XRANGE, sin(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) / YRANGE, cos(_zDegrees / 360 * 2 * M_PI) / ZRANGE);
    Point planePoint(2 * cos(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) * XRANGE, 2 * sin(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) * YRANGE, 2 * cos(_zDegrees / 360 * 2 * M_PI) * ZRANGE);
    Point drawablePoint(0,0,0);
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
            drawablePoint = projectToViewPlane(x, y, result, NormalVector, planePoint);
            drawPoint(drawablePoint._x, drawablePoint._y);
        }
    }

    //choose 10 y points and evaluate each at every x
    for (double y = -(double)YRANGE / 2; y <= (double)YRANGE / 2; y += 10 * dy) {
        for (double x = -(double)XRANGE / 2; x <= (double)XRANGE / 2; x += dx) {
            Equation eq;
            eq.translate(equationString);
            result = eq.evaluate({ (float)x,(float)y });
            drawablePoint = projectToViewPlane(x, y, result, NormalVector, planePoint);
            drawPoint(drawablePoint._x, drawablePoint._y);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

Point ThreeDGraph::projectToViewPlane(double x, double y, double z, Point NormalVector, Point planePoint) {
    Point pointToProject(x, y, z);
    Point projectedPoint = pointToProject - (NormalVector * ((NormalVector * (pointToProject - planePoint)) * (1 / (NormalVector * NormalVector))));
    planePoint.toSpherical();
    projectedPoint.toSpherical();
    projectedPoint._z -= planePoint._z;
    projectedPoint.toRectangular();
    return projectedPoint;

    /*Point relativeProjectedPoint = projectedPoint - planePoint;
    Point AlignedPoint(2*XRANGE * cos(_xDegrees / 360 * 2 * M_PI) / sin(_zDegrees / 360 * 2 * M_PI), 2*YRANGE * sin(_xDegrees / 360 * 2 * M_PI) / sin(_zDegrees / 360 * 2 * M_PI), 0);
    AlignedPoint = AlignedPoint - planePoint;

    double angle = acos((AlignedPoint * relativeProjectedPoint)/((AlignedPoint * AlignedPoint)*(relativeProjectedPoint * relativeProjectedPoint)));
    double magnitude = relativeProjectedPoint * relativeProjectedPoint;
    */
}
void zoom() {}

void rotate() {

}

void ThreeDGraph::setBounds() {
    Point NormalVector(cos(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) / XRANGE, sin(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) / YRANGE, cos(_zDegrees / 360 * 2 * M_PI) / ZRANGE);
    Point planePoint(2 * cos(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) * XRANGE, 2 * sin(_xDegrees / 360 * 2 * M_PI) * sin(_zDegrees / 360 * 2 * M_PI) * YRANGE, 2 * cos(_zDegrees / 360 * 2 * M_PI) * ZRANGE);
    Point corner = projectToViewPlane(-XRANGE, -YRANGE, -ZRANGE, NormalVector, planePoint);
    double lowestX = corner._x;
    double lowestY = corner._y;
    double highestX = corner._x;
    double highestY = corner._y;
    for (int i = 1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int k = -1; k <= 1; k += 2) {
                corner = projectToViewPlane(XRANGE * i, YRANGE * j, ZRANGE * k, NormalVector, planePoint);
                if (corner._x <= lowestX) {
                    lowestX = corner._x;
                }

                if (corner._y <= lowestY) {
                    lowestY = corner._y;
                }
                if (corner._x >= highestX) {
                    highestX = corner._x;
                }

                if (corner._y >= highestY) {
                    highestY = corner._y;
                }
            }
        }
    }
    graphingWindow.maxX = highestX;
    graphingWindow.maxY =highestY;
    graphingWindow.minX = lowestX;
    graphingWindow.minY = highestY;
    graphingWindow.WINDOW_HEIGHT = WINDOW_HEIGHT;
    graphingWindow.WINDOW_WIDTH = WINDOW_WIDTH;
}


