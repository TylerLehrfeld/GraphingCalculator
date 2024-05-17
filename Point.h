#include <iostream>

using namespace std;

class Point {
    public:
    Point(double x, double y, double z) {
        _x = x;
        _y = y;
        _z = z;
    }
    Point() {
        _x = 0;
        _y = 0;
        _z = 0;
    }

    void rotateZ(double angle) {
        double x = _x*cos(angle) - _y*sin(angle);
        double y = _x*sin(angle) + _y*cos(angle);
        _x = x;
        _y = y;
    }

    void rotateX(double angle) {
        double x = _x;
        double y = _y * cos(angle) - _z*sin(angle);
        double z = _y * sin(angle) + _z*cos(angle);
        _x = x;
        _y = y;
        _z = z;
    }

    void rotateY(double angle) {
        double x = cos(angle) * _x + sin(angle) * _z;
        double y = _y;
        double z = -sin(angle) * _x + cos(angle) * _z;
        _x = x;
        _y = y;
        _z = z;
    }

    Point operator+(Point p1) {
        Point res(0,0,0);
        res._x = _x + p1._x;
        res._y = _y + p1._y;
        res._z = _z + p1._z;
        return res;
    }

    Point operator-(Point p1) {
        Point res(0,0,0);
        res._x = _x - p1._x;
        res._y = _y - p1._y;
        res._z = _z - p1._z;
        return res;
    }

    double operator*(Point p1) {
        double res = 0;
        res += p1._x * _x;
        res += p1._y * _y;
        res += p1._z * _z;
        return res;
    }
    Point operator*(double scalar) {
        Point res(_x,_y,_z);
        res._x *= scalar;
        res._y *= scalar;
        res._z *= scalar;
        return res;
    }
    void print() {
        cout << "("<<_x<<", "<<_y<<", "<<_z<<")" << endl;
    }

    void toSpherical() {
        double rho = sqrt(pow(_x,2)+pow(_y,2)+pow(_z,2));
        double theta = atan2(_y,_x);
        double phi = acos(_z/rho);
        _x = rho;
        _y = theta;
        _z = phi;
    }

    void toRectangular() {
        double x = _x*sin(_z)*cos(_y);
        double y = _x*sin(_z)*sin(_y);
        double z = _x*cos(_z);
        _x = x;
        _y = y;
        _z = z;
    }
    double _x;
    double _y;
    double _z;
};