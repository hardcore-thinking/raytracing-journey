#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>

#include "Hittable.hpp"
#include "Color.hpp"

class Material {
	public:
		virtual ~Material() = default;

		virtual bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const;
};

class Lambertian : public Material {
	public:
		Lambertian(Color const& albedo);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;

	private:
		Color albedo = {};
};

class Metal : public Material {
	public:
		Metal(Color const& albedo, double fuzz);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;

	private:
		Color albedo = {};
		double fuzz = 0.0;
};

class Dielectric : public Material {
	public:
		Dielectric(double refractionIndex);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;
		static double Reflectance(double cosine, double refractionIndex);

	private:
		double refractionIndex = 0.0;
};

#endif // MAATERIAL_HPP