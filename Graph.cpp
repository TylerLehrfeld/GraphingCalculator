#include "Graph.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include "statisticalOperations.h"

using namespace std;

Graph::~Graph(){

};

Graph::Graph()
{
    pixels = nullptr;
    pitch = 0;
}

void Graph::init()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
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
    if (window == nullptr)
    {
        printf("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // draws the axis lines
    drawAxis();

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                string inputEquation;
                cout << "Enter your equation here" << endl;
                cin >> inputEquation;
                graphLine(inputEquation, false, 0, LINE);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                char keyPressed = (char)(event.key.keysym.sym);
                if (keyPressed == 'n')
                {
                    normal();
                }
                else if (keyPressed == 't')
                {
                    tTest();
                }
                else if (keyPressed == 'c')
                {
                    chiSquaredTest();
                }
            }
            SDL_Delay(20);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graph::graphLine(string equationString, bool isCDF, float testStatistic, int testNumber)
{
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
    {
        cout << "error in graph line: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    bool lastHeightInitialized = false;
    int lastHeight = 0;
    for (int i = 0; i < WINDOW_WIDTH; i++)
    {
        Equation *equation = new Equation();
        equation->translate(equationString);
        float xVal = (float)((float)(-WINDOW_WIDTH / 2 + i) / (float)WINDOW_WIDTH * (float)XRANGE);
        vector<float> variable_values{xVal};
        
        float result = (equation->evaluate(variable_values));

        // cout << "input x = " << variable_values[0] << " result: " << result << endl;
        result = result / (float)YRANGE * (float)(WINDOW_HEIGHT);

        result += WINDOW_HEIGHT / 2;
        if (result < WINDOW_HEIGHT && result > 0)
        {
            // draw next pixel from the last one
            if (!lastHeightInitialized)
            {
                lastHeight = result;
                lastHeightInitialized = true;
                Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)result) * (pitch / sizeof(Uint32));
                *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
            }
            else
            {
                if (lastHeight <= result)
                {
                    for (int j = lastHeight; j <= result; j++)
                    {
                        Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)j) * (pitch / sizeof(Uint32));
                        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
                    }
                }
                else
                {
                    for (int j = result; j <= lastHeight; j++)
                    {
                        Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)j) * (pitch / sizeof(Uint32));
                        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
                    }
                }
                lastHeight = result;
            }

            // draw red line from the pixel to the x axis if we are demonstrating a cdf
            if (isCDF &&
                ((AlternativeHypNumber == 1 && (xVal <= -1 * testStatistic || xVal >= testStatistic)) ||
                 (AlternativeHypNumber == 2 && xVal >= testStatistic) ||
                 (AlternativeHypNumber == 3 && xVal <= -1 * testStatistic)))
            {
                for (int h = WINDOW_HEIGHT / 2 + 1; h < lastHeight; h++)
                {
                    Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + (WINDOW_HEIGHT - (int)h) * (pitch / sizeof(Uint32));
                    *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 0, 0);
                }
            }
        }
        variable_values.clear();
        delete equation;
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Graph::drawAxis()
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
    {
        cout << "error in draw axis: " << SDL_GetError() << endl;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    for (int i = 0; i < WINDOW_HEIGHT; i++)
    {
        Uint32 *pixel = static_cast<Uint32 *>(pixels) + WINDOW_WIDTH / 2 + i * (pitch / sizeof(Uint32));
        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
    }
    for (int i = 0; i < WINDOW_WIDTH; i++)
    {
        Uint32 *pixel = static_cast<Uint32 *>(pixels) + i + WINDOW_HEIGHT / 2 * (pitch / sizeof(Uint32));
        *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
    }
    SDL_UnlockTexture(texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

// 0,0 is top left.
// WINDOW_WIDTH-1,WINDOW_HEIGHT-1 is bottom right
void Graph::drawPixel(int x, int y)
{
    if (x < 0 || x >= WINDOW_WIDTH)
    {
        cout << "width out of bounds: " << x << (x > 0 ? " is greater than " : " is less than ") << (x > 0 ? WINDOW_WIDTH - 1 : 0) << endl;
        return;
    }
    if (y < 0 || y >= WINDOW_HEIGHT)
    {
        cout << "height out of bounds: " << y << (y > 0 ? " is greater than " : " is less than ") << (y > 0 ? WINDOW_HEIGHT - 1 : 0) << endl;
        return;
    }
    Uint32 *pixel = static_cast<Uint32 *>(pixels) + x + y * (pitch / sizeof(Uint32));
    *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255);
}

void Graph::tTest()
{

    int k = 0;
    int n = 0;
    double sampleMean = 0;
    double sampleStandardDeviation = 0;
    double mean = 0;
    float a = 0;
    cout << "starting one sample T-test" << endl;
    cout << "Choose Alpha: " << endl;
    cin >> a;
    cout << "Choose alternative hypothesis: " << endl
         << "1. µ ≠ µ₀" << endl
         << "2. µ > µ₀" << endl
         << "3. µ < µ₀" << endl;
    cin >> AlternativeHypNumber;
    cout << "enter sample mean: ";
    cin >> sampleMean;
    cout << "enter sample standard deviation: ";
    cin >> sampleStandardDeviation;
    cout << "enter hypothesis mean: ";
    cin >> mean;
    cout << "enter sample size: ";
    cin >> n;
    // k is degrees of freedom
    k = n - 1;
    double dividend = tgamma((k + 1) / 2);
    double divisor = tgamma((double)k / 2.0) * sqrt(k * M_PI);
    double multiplier = dividend / divisor;
    double exponent = -.5 * (k + 1);
    double testStatistic = abs((sampleMean - mean) / (sampleStandardDeviation / sqrt(n)));
    double area = 1 - tCDF(testStatistic, k);
    if (AlternativeHypNumber == 1)
    {
        area *= 2;
    }

    stringstream ss;
    ss << multiplier << "*(1+(x^2/" << k << "))^" << exponent;
    cout << "your test statistic is " << testStatistic << endl;
    cout << "the area under the curve is " << area << endl;
    if (area <= a)
    {
        cout << "Because the area is less than " << a << ", your results are statistically significant and the alternative hypothesis is accepted" << endl;
    }
    else
    {
        cout << "Your area is greater than " << a << ", your null hypothesis cannot be rejected." << endl;
    }
    graphLine(ss.str(), true, testStatistic, T_TEST);
}

void Graph::normal()
{
    cout << "drawing normal curve" << endl
         << "enter mean: ";
    float mean = 0;
    cin >> mean;
    cout << "enter variance: ";
    float variance = 0;
    cin >> variance;
    stringstream ss;
    ss << "(1/(" << variance << "*(2*3.14159)^.5))*2.71828^(-.5*((x-" << mean << ")/" << variance << ")^2)";
    graphLine(ss.str(), false, 0, NORMAL);
}

void Graph::chiSquaredTest() {
    cout << "starting Chi-Squared test" << endl;
    cout << "enter degrees of freedom" << endl;
    int k;
    cin >> k;
    float factkovertwo = tgamma((double)k/2);
    AlternativeHypNumber = 2;
    graphLine("(x^(k/2-1)*e^(-x/2))/(2^(k/2)*)", true, 20, CHI_SQUARED);
}