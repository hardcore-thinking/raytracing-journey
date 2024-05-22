#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Color.hpp"
#include "Vec3.hpp"

class Texture {
	public:
		virtual ~Texture() = default;

		virtual Color Value(double u, double v, Point3 const& p) const = 0;
};

class SolidColor : public Texture {
	public:
		SolidColor(Color const& albedo);
		SolidColor(double red, double green, double blue);

		Color Value(double u, double v, Point3 const& p) const override;

	private:
		Color _albedo = {};
};
	
#endif // TEXTURE_HPP