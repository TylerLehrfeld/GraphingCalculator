#include "3DGraph.h"
#include "drawTriangle.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include <thread>

ThreeDGraph::ThreeDGraph() {
    pixels = nullptr;
    pitch = 0;
    Normal._x = 0;
    Normal._y = 0;
    Normal._z = 1;
}

ThreeDGraph::~ThreeDGraph() {
    equationList.clear();
}

void ThreeDGraph::drawPoint(double x, double y, int r, int g, int b) {
    Point pixelPoint = graphingWindow.toPixel(x, y);
    x = pixelPoint._x;
    y = pixelPoint._y;
    if ((int)x <= 0 || (int)x >= WINDOW_WIDTH || (int)y <= 0 || (int)y > WINDOW_WIDTH) {
        return;
    } else {

        Uint32* pixel = static_cast<Uint32*>(pixels) + (int)(x)+(int)(WINDOW_HEIGHT - y) * (pitch / sizeof(Uint32));
        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), r, g, b);

    }
}

void ThreeDGraph::drawAxis() {
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        cout << "error in graph line: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    double dx = (double)XRANGE * 2 / 1000;
    double dy = (double)YRANGE * 2 / 1000;
    double dz = (double)ZRANGE * 2 / 1000;
    double x = 0;
    double y = 0;
    double z = 0;
    for (x = -(double)XRANGE; x <= (double)XRANGE; x += dx) {
        Point drawablePoint = projectToViewPlane(x, 0, 0);
        drawPoint(drawablePoint._x, drawablePoint._y, 255, 0, 0);
    }
    for (y = -(double)YRANGE; y <= (double)YRANGE; y += dy) {
        Point drawablePoint = projectToViewPlane(0, y, 0);
        drawPoint(drawablePoint._x, drawablePoint._y, 0, 0, 255);
    }
    for (z = -(double)ZRANGE; z <= (double)ZRANGE; z += dz) {
        Point drawablePoint = projectToViewPlane(0, 0, z);
        drawPoint(drawablePoint._x, drawablePoint._y, 0, 255, 0);
    }
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

}

#include "GraphHelper.h"


void ThreeDGraph::initializeSDLVariables() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(
        "Click l to enter equation in console",
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
                    cout << "Enter your equation with variables x and y:" << endl;
                    string equationString;
                    cin >> equationString;
                    equationList.push_back(new NewEquationParser(equationString));
                    for (NewEquationParser* parser : equationList) {
                        addSurface(parser);
                    }
                } else if ((int)keyPressed == 79) {
                    rotate(0, .1);
                } else if ((int)keyPressed == 80) {
                    rotate(0, -.1);
                } else if ((int)keyPressed == 81) {
                    rotate(-.1, 0);
                } else if ((int)keyPressed == 82) {
                    rotate(.1, 0);
                } else if ((int)keyPressed == 61) {
                    zoom(-1);//zoom in
                } else if ((int)keyPressed == 45) {
                    zoom(1);
                }
            }

        }
    }
}

void drawXContourLines(NewEquationParser* parser, SDL_Texture* texture, int pitch, void* pixels, ThreeDGraph* graph, int XRANGE, int YRANGE) {
    //We want to create 10 contour X and Y contourlines for the range selected
    //so we choose 10 x points and evaluate all of the y points to get a contour line
    Point drawablePoint(0, 0, 0);
    double dx = (double)XRANGE * 2 / 1000;
    double dy = (double)YRANGE * 2 / 1000;

    for (double x = -(double)XRANGE; x <= (double)XRANGE; x += 10 * dx) {
        for (double y = -(double)YRANGE; y <= (double)YRANGE; y += dy) {
            float result = parser->evaluate(x, y);
            drawablePoint = graph->projectToViewPlane(x, y, result);
            graph->drawPoint(drawablePoint._x, drawablePoint._y, 255, 255, 255);
        }
    }
}

void drawYContourLines(NewEquationParser* parser, SDL_Texture* texture, int pitch, void* pixels, ThreeDGraph* graph, int XRANGE, int YRANGE) {
    //choose 10 y points and evaluate each at every x
    Point drawablePoint(0, 0, 0);
    double dx = (double)XRANGE * 2 / 1000;
    double dy = (double)YRANGE * 2 / 1000;

    for (double y = -(double)YRANGE; y <= (double)YRANGE; y += 10 * dy) {
        for (double x = -(double)XRANGE; x <= (double)XRANGE; x += dx) {
            float result = parser->evaluate(x, y);
            drawablePoint = graph->projectToViewPlane(x, y, result);
            graph->drawPoint(drawablePoint._x, drawablePoint._y, 255, 255, 255);
        }
    }
}


void ThreeDGraph::addSurface(NewEquationParser* parser) {
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        cout << "error in graph line: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    std::thread xThread(drawXContourLines, parser, texture, pitch, pixels, this, XRANGE, YRANGE);
    std::thread yThread(drawYContourLines, parser, texture, pitch, pixels, this, XRANGE, YRANGE);

    xThread.join();
    yThread.join();
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

Point ThreeDGraph::projectToViewPlane(double x, double y, double z) {
    Point pointToProject(x, y, z);
    Point projectedPoint = pointToProject - (Normal * ((Normal * (pointToProject)) * (1 / (Normal * Normal))));
    projectedPoint.rotateZ(-thetaZ);
    projectedPoint.rotateX(-thetaX);
    return projectedPoint;

}

void ThreeDGraph::rotate(double xDegrees, double zDegrees) {
    SDL_RenderClear(renderer);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    thetaX += xDegrees;
    thetaZ += zDegrees;
    setBounds();
    drawAxis();
    for (NewEquationParser* parser : equationList) {
        addSurface(parser);
    }
}

void ThreeDGraph::setBounds() {
    Normal._x = 0;
    Normal._y = 0;
    Normal._z = 1;
    Normal.rotateX(thetaX);
    Normal.rotateZ(thetaZ);
    Point corner = projectToViewPlane(-XRANGE, -YRANGE, ZRANGE);
    double lowestX = corner._x;
    double lowestY = corner._y;
    double highestX = corner._x;
    double highestY = corner._y;
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int k = -1; k <= 1; k += 2) {
                corner = projectToViewPlane(XRANGE * i, YRANGE * j, ZRANGE * k);
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
    graphingWindow.maxY = highestY;
    graphingWindow.minX = lowestX;
    graphingWindow.minY = lowestY;
    graphingWindow.WINDOW_HEIGHT = WINDOW_HEIGHT;
    graphingWindow.WINDOW_WIDTH = WINDOW_WIDTH;
}

void ThreeDGraph::zoom(double zoomDiff) {
    SDL_RenderClear(renderer);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    XRANGE += zoomDiff;
    YRANGE += zoomDiff;
    ZRANGE += zoomDiff;
    setBounds();
    drawAxis();
    for (NewEquationParser* parser : equationList) {
        addSurface(parser);
    }
}

