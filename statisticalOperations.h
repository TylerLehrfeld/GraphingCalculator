#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/chi_squared.hpp>

double tCDF(double testStat, int degreeOfFreedom) {
    boost::math::students_t dist(degreeOfFreedom);
    return boost::math::cdf(dist, testStat);
}

double chiCDF(double testStat, int degreesOfFreedom) {
    boost::math::chi_squared dist(degreesOfFreedom);
    return boost::math::cdf(dist, testStat);
}