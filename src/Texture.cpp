#include "Texture.hpp"

SolidColor::SolidColor(Color const& albedo) : _albedo(albedo) {}

SolidColor::SolidColor(double red, double green, double blue) : _albedo(Color(red, green, blue)) {}

Color SolidColor::Value(double u, double v, Point3 const& p) const {
	return _albedo;
}