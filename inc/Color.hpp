#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "Vec3.hpp"
#include "Interval.hpp"

using Color = Vec3;

double LinearToGamma(double linearComponent);
void WriteColor(std::ostream& out, Color const& pixelColor);

#endif // COLOR_HPP
