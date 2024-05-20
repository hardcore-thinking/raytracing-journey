#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>

#include "Hittable.hpp"
#include "Material.hpp"

class Sphere : public Hittable {
public:
	Sphere(Point3 const& center, double radius, std::shared_ptr<Material> mat);

	bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;

private:
	Point3 center = {};
	double radius = 0.0;
	std::shared_ptr<Material> mat = nullptr;
};

#endif // SPHERE_HPP