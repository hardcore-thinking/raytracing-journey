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

ImageTexture::ImageTexture(std::string const& filename) : _image(filename) {}

Color ImageTexture::Value(double u, double v, Point3 const& p) const {
	// If we have no texture data, then return solid cyan as a debugging aid
	if (_image.Height() <= 0) {
		return Color(0, 1, 1);
	}

	// Clamp input texture coordinates to [0, 1] x [0, 1]
	u = Interval(0, 1).Clamp(u);
	v = 1.0 - Interval(0, 1).Clamp(v); // Flip v to immage coordinates

	auto i = static_cast<int>(u * _image.Width());
	auto j = static_cast<int>(v * _image.Height());
	auto pixel = _image.PixelData(i, j);

	auto colorScale = 1.0 / 255.0;
	
	return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
}

NoiseTexture::NoiseTexture(double scale) : _scale(scale) {}

Color NoiseTexture::Value(double u, double v, Point3 const& p) const {
	return Color(1, 1, 1) * _noise.Noise(_scale * p);
}