#ifndef QUAD_HPP
#define QUAD_HPP

#include <memory>

#include "Hittable.hpp"
#include "Vec3.hpp"
#include "Material.hpp"
#include "Interval.hpp"
#include "Ray.hpp"
#include "AABB.hpp"
#include "HittableList.hpp"

class Quad : public Hittable {
	public:
		Quad(Point3 const& q, Vec3 const& u, Vec3 const& v, std::shared_ptr<Material> mat);

		virtual void SetBoundingBox();

		AABB BoundingBox() const override;
		bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;

		bool IsInterior(double a, double b, HitRecord& rec) const;

	private:
		Point3 _q = {};
		Vec3 _u = {};
		Vec3 _v = {};
		Vec3 _w = {};
		std::shared_ptr<Material> _mat = nullptr;
		AABB _bBox = {};
		Vec3 _normal = {};
		double _d = 0.0;
};

std::shared_ptr<HittableList> Box(Point3 const& a, Point3 const& b, std::shared_ptr<Material> mat);

#endif // QUAD_HPP