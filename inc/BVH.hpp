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
};

#endif // BVH_HPP