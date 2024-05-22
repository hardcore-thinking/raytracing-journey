#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>
#include <memory>

#include "Hittable.hpp"
#include "Color.hpp"
#include "Texture.hpp"

class Material {
	public:
		virtual ~Material() = default;

		virtual bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const;
};

class Lambertian : public Material {
	public:
		Lambertian(Color const& albedo);
		Lambertian(std::shared_ptr<Texture> tex);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;

	private:
		std::shared_ptr<Texture> _tex;
};

class Metal : public Material {
	public:
		Metal(Color const& albedo, double fuzz);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;

	private:
		Color _albedo = {};
		double _fuzz = 0.0;
};

class Dielectric : public Material {
	public:
		Dielectric(double refractionIndex);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;
		static double Reflectance(double cosine, double refractionIndex);

	private:
		double _refractionIndex = 0.0;
};

#endif // MAATERIAL_HPP