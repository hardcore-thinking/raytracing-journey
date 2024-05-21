#ifndef BVH_HPP
#define BVH_HPP

#include <vector>
#include <memory>

#include "AABB.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"

class BVHNode : public Hittable {
	public:
		BVHNode(HittableList list);
		BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);
		bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;
		AABB BoundingBox() const override;

	private:
		std::shared_ptr<Hittable> _left = nullptr;
		std::shared_ptr<Hittable> _right = nullptr;
		AABB _bBox = {};

		static bool BoxCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b, int axisIndex);
		static bool BoxXCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b);
		static bool BoxYCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b);
		static bool BoxZCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b);
};

#endif // BVH_HPP