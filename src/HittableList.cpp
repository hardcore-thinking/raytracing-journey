#include "HittableList.hpp"

HittableList::HittableList() {};
HittableList::HittableList(std::shared_ptr<Hittable> object) { Add(object); }

void HittableList::Clear() {
	objects.clear();
}

void HittableList::Add(std::shared_ptr<Hittable> object) {
	objects.push_back(object);
	_bBox = AABB(_bBox, object->BoundingBox());
}

bool HittableList::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	auto closestSoFar = rayT.max;

	for (auto const& object : objects) {
		if (object->Hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}

	return hitAnything;
}

AABB HittableList::BoundingBox() const {
	return _bBox;
}