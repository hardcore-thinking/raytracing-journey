#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>

#include "Vec3.hpp"
#include "Ray.hpp"
#include "Interval.hpp"
#include "AABB.hpp"

class Material;

class HitRecord {
	public:
		Point3 p = {};
		Vec3 normal = {};
		std::shared_ptr<Material> mat = nullptr;
		double t = 0.0;
		double u = 0.0;
		double v = 0.0;
		bool frontFace = false;

		void SetFaceNormal(Ray const& r, Vec3 const& outwardNormal);
};

class Hittable {
	public:
		virtual ~Hittable() = default;

		virtual bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const = 0;
		virtual AABB BoundingBox() const = 0;
};

class Translate : public Hittable {
	public:
		Translate(std::shared_ptr<Hittable> object, Vec3 const& offset);

		bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;
		AABB BoundingBox() const override;

	private:
		std::shared_ptr<Hittable> _object = nullptr;
		Vec3 _offset = {};
		AABB _bBox = {};
};

#endif // HITTABLE_HPP
