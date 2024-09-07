#include "Point.h"

#ifndef TRIANGLE
#define TRIANGLE

class Triangle {
public:
    double dist(Point& Normal);
    Triangle(Point& a, Point& b, Point& c);
    Point *A, *B, *C;
private:
    Point closestPoint(Point& Normal);
    Point Closest;


};
#endif