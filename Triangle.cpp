#include "Triangle.h"

Point Triangle::closestPoint(Point& Normal) {
    double Adist = (*A * (Normal * *A)) * (*A * (Normal * *A));
    double Bdist = (*B * (Normal * *B)) * (*B * (Normal * *B));
    double Cdist = (*C * (Normal * *C)) * (*C * (Normal * *C));
    if(Adist > Bdist && Adist > Cdist) {
        return *A;
    } else if(Bdist > Cdist) {
        return *B;
    } else {
        return *C;
    }

}

Triangle::Triangle(Point& a, Point& b, Point& c) {
    A = &a;
    B = &b;
    C = &c;
}

double Triangle::dist(Point& Normal) {
    Closest = closestPoint(Normal);
    return  (Closest * Normal) * (Closest * Closest);
}