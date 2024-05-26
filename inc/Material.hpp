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
		virtual Color Emitted(double u, double v, Point3 const& p) const;
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

class DiffuseLight : public Material {
	public:
		DiffuseLight(std::shared_ptr<Texture> tex);
		DiffuseLight(Color const& emit);

		Color Emitted(double u, double v, Point3 const& p) const override;
		
	private:
		std::shared_ptr<Texture> _tex = nullptr;
};

class Isotropic : public Material {
	public:

	private:
		Isotropic(Color const& albedo);
		Isotropic(std::shared_ptr<Texture> tex);

		bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override;

	private:
		std::shared_ptr<Texture> _tex = nullptr;
};

#endif // MAATERIAL_HPP