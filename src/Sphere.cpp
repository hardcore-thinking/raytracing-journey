#include "Sphere.hpp"

Sphere::Sphere(Point3 const& center, double radius, std::shared_ptr<Material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}

bool Sphere::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	Vec3 oc = center - r.Origin();

	auto a = r.Direction().LengthSquared();
	auto h = Dot(r.Direction(), oc);
	auto c = oc.LengthSquared() - radius * radius;

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
	Vec3 outwardNormal = (rec.p - center) / radius;
	rec.SetFaceNormal(r, outwardNormal);
	rec.mat = mat;

	return true;
}