#include "Color.hpp"

double LinearToGamma(double linearComponent) {
	if (linearComponent > 0) {
		return std::sqrt(linearComponent);
	}

	return 0;
}

void WriteColor(std::ostream& out, Color const& pixelColor) {
	auto r = pixelColor.X();
	auto g = pixelColor.Y();
	auto b = pixelColor.Z();

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	// translates the [0,1] component values to byte range [0,255]
	static Interval const intensity(0.000, 0.999);
	int rByte = static_cast<int>(256 * intensity.Clamp(r));
	int gByte = static_cast<int>(256 * intensity.Clamp(g));
	int bByte = static_cast<int>(256 * intensity.Clamp(b));

	// Write out the pixel components
	out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}