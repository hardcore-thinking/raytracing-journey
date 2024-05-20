#include "Material.hpp"

bool Material::Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const {
	return false;
}

Lambertian::Lambertian(Color const& albedo) : _albedo(albedo) {}

bool Lambertian::Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const {
	auto scatterDirection = rec.normal + RandomUnitVector();

	if (scatterDirection.NearZero()) {
		scatterDirection = rec.normal;
	}

	scattered = Ray(rec.p, scatterDirection);
	attenuation = _albedo;
	return true;
}

Metal::Metal(Color const& albedo, double fuzz) : _albedo(albedo), _fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

bool Metal::Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const {
	Vec3 reflected = Reflect(rIn.Direction(), rec.normal);
	reflected = UnitVector(reflected) + (_fuzz * RandomUnitVector());
	scattered = Ray(rec.p, reflected);
	attenuation = _albedo;
	return (Dot(scattered.Direction(), rec.normal) > 0);
}

Dielectric::Dielectric(double refractionIndex) : _refractionIndex(refractionIndex) {}

bool Dielectric::Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const {
	attenuation = Color(1.0, 1.0, 1.0);
	double ri = rec.frontFace ? (1.0 / _refractionIndex) : _refractionIndex;

	Vec3 unitDirection = UnitVector(rIn.Direction());
	double cosTheta = std::fmin(Dot(-unitDirection, rec.normal), 1.0);
	double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

	bool cannotRefract = ri * sinTheta > 1.0;
	Vec3 direction = {};

	if (cannotRefract || Reflectance(cosTheta, ri) > RandomDouble()) {
		direction = Reflect(unitDirection, rec.normal);
	}

	else {
		direction = Refract(unitDirection, rec.normal, ri);
	}

	scattered = Ray(rec.p, direction);
	return true;
}

double Dielectric::Reflectance(double cosine, double refractionIndex) {
	auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
	r0 = r0 * r0;
	return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}