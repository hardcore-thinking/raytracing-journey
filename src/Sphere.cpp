#include "Sphere.hpp"

Sphere::Sphere(Point3 const& center, double radius, std::shared_ptr<Material> mat) : _center(center), _radius(std::fmax(0, radius)), _mat(mat) {}

bool Sphere::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	Vec3 oc = _center - r.Origin();

	auto a = r.Direction().LengthSquared();
	auto h = Dot(r.Direction(), oc);
	auto c = oc.LengthSquared() - _radius * _radius;

	auto discriminant = h * h - a * c;

	if (discriminant < 0) {
		return false;
	}

	auto sqrtd = std::sqrt(discriminant);

	auto root = (h - sqrtd) / a;
	if (!rayT.Surrounds(root)) {
		root = (h + sqrtd) / a;
		if (!rayT.Surrounds(root)) {
			return false;
		}
	}

	rec.t = root;
	rec.p = r.At(rec.t);
	Vec3 outwardNormal = (rec.p - _center) / _radius;
	rec.SetFaceNormal(r, outwardNormal);
	rec.mat = _mat;

	return true;
}