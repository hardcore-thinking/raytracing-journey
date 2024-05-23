#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>
#include <memory>

#include "Color.hpp"
#include "Vec3.hpp"
#include "RTWSTBImage.hpp"

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

class CheckerTexture : public Texture {
	public:
		CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd);
		CheckerTexture(double scale, Color const& c1, Color const& c2);

		Color Value(double u, double v, Point3 const& p) const override;

	private:
		double _invScale = 0.0;
		std::shared_ptr<Texture> _even = nullptr;
		std::shared_ptr<Texture> _odd  = nullptr;
};

class ImageTexture : public Texture {
	public:
		ImageTexture(std::string const& filename);

		Color Value(double u, double v, Point3 const& p) const override;

	private:
		RTWImage _image;
};
	
#endif // TEXTURE_HPP