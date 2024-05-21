#include "BVH.hpp"

BVHNode::BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {

}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {

}

bool BVHNode::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	if (_bBox.Hit(r, rayT)) {
		return false;
	}

	bool hitLeft = _left->Hit(r, rayT, rec);
	bool hitRight = _right->Hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

	return hitLeft || hitRight;
}

AABB BVHNode::BoundingBox() const {
	return _bBox;
}