#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <limits>
#include <random>

// Constants
double const infinity = std::numeric_limits<double>::infinity();
double const pi = 3.1415926535897932385;

// Utility functions
double DegreesToRadians(double degrees);
double RandomDouble();
double RandomDouble(double min, double max);

#endif // RTWEEKEND_HPP