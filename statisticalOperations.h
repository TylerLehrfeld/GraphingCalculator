#include <boost/math/distributions/students_t.hpp>

double tCDF(double testStat, int degreeOfFreedom) {
    boost::math::students_t dist(degreeOfFreedom);
    return boost::math::cdf(dist, testStat);
}