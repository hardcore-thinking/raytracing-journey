#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <memory>
#include <vector>

#include "Hittable.hpp"

class HittableList : public Hittable {
	public:
		std::vector<std::shared_ptr<Hittable>> objects = {};

		HittableList();
		HittableList(std::shared_ptr<Hittable> object);

		void Clear();
		void Add(std::shared_ptr<Hittable> object);

		bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;
		AABB BoundingBox() const override;

	private:
		AABB _bBox = {};
};

#endif // HITTABLE_LIST_HPP
