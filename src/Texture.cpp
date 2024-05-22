#include "Texture.hpp"

SolidColor::SolidColor(Color const& albedo) : _albedo(albedo) {}

SolidColor::SolidColor(double red, double green, double blue) : _albedo(Color(red, green, blue)) {}

Color SolidColor::Value(double u, double v, Point3 const& p) const {
	return _albedo;
}

CheckerTexture::CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
	: _invScale(1.0 / scale), _even(even), _odd(odd) {}

CheckerTexture::CheckerTexture(double scale, Color const& c1, Color const& c2)
	: _invScale(1.0 / scale), _even(std::make_shared<SolidColor>(c1)), _odd(std::make_shared<SolidColor>(c2)) {}

Color CheckerTexture::Value(double u, double v, Point3 const& p) const {
	auto xInteger = static_cast<int>(std::floor(_invScale * p.X()));
	auto yInteger = static_cast<int>(std::floor(_invScale * p.Y()));
	auto zInteger = static_cast<int>(std::floor(_invScale * p.Z()));

	bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

	return isEven ? _even->Value(u, v, p) : _odd->Value(u, v, p);
}