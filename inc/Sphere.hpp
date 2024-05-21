#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>

#include "Hittable.hpp"
#include "Material.hpp"
#include "AABB.hpp"

class Sphere : public Hittable {
	public:
		// Stationary Sphere
		Sphere(Point3 const& center, double radius, std::shared_ptr<Material> mat);

		// Moving Sphere
		Sphere(Point3 const& center1, Point3 const& center2, double radius, std::shared_ptr<Material> mat);

		bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;
		AABB BoundingBox() const override;

	private:
		Point3 _center1 = {};
		double _radius = 0.0;
		std::shared_ptr<Material> _mat = nullptr;
		bool _isMoving = false;
		Vec3 _centerVec = {};
		AABB _bBox = {};

		Point3 SphereCenter(double time) const;
};

#endif // SPHERE_HPP