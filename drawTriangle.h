#ifndef DRAW_TRIANGLE
#define DRAW_TRIANGLE

double invSqrt(double x) {
    return 1 / sqrt(x);
}

void ThreeDGraph::drawTriangle(Point one, Point two, Point three) {

    Point normal(0, 0, 1);
    Point vector1 = one - two;
    Point vector2 = one - three;
    Point crossProduct = (vector1 % vector2);
    Point normalizedCrossProduct = crossProduct * invSqrt(crossProduct * crossProduct);
    Uint8 shade = (int)(normal * normalizedCrossProduct * 255);

    one = projectToViewPlane(one._x, one._y, one._z);
    one = graphingWindow.toPixel(one._x, one._y);
    two = projectToViewPlane(two._x, two._y, two._z);
    two = graphingWindow.toPixel(two._x, two._y);
    three = projectToViewPlane(three._x, three._y, three._z);
    three = graphingWindow.toPixel(three._x, three._y);
    int minX = min(one._x, min(two._x, three._x));
    int minY = min(one._y, min(two._y, three._y));
    int maxX = max(one._x, max(two._x, three._x));
    int maxY = max(one._y, max(two._y, three._y));

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Point p(x, y, 0);
            float crossone = ((p - one) % (two - one))._z;
            float crosstwo = ((p - two) % (three - two))._z;
            float crossthree = ((p - three) % (one - three))._z;
            bool inTriangle = ((crossone > 0) && (crosstwo > 0) && (crossthree > 0)) ||
                ((crossone < 0) && (crosstwo < 0) && (crossthree < 0));
            if (!(((int)x <= 0 || (int)x >= WINDOW_WIDTH || (int)y <= 0 || (int)y > WINDOW_WIDTH) || !inTriangle)) {
                Uint32* pixel = static_cast<Uint32*>(pixels) + (int)(x)+(int)(WINDOW_HEIGHT - y) * (pitch / sizeof(Uint32));
                *pixel = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), shade, shade, shade);
            }
        }
    }
}


#endif
