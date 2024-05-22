#include "BVH.hpp"

#include "Utils.hpp"

BVHNode::BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {

}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
	// Build the bounding box of the span of source objects.
	_bBox = AABB::empty;
	for (size_t objectIndex = start; objectIndex < end; objectIndex++) {
		_bBox = AABB(_bBox, objects[objectIndex]->BoundingBox());
	}

	int axis = _bBox.LongestAxis();

	auto comparator = (axis == 0) ? BoxXCompare
		            : (axis == 1) ? BoxYCompare
		                          : BoxZCompare;

	size_t objectSpan = end - start;

	if (objectSpan == 1) {
		_left = _right = objects[start];
	}

	else if (objectSpan == 2) {
		_left = objects[start];
		_right = objects[start + 1];
	}

	else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		auto mid = start + objectSpan / 2;
		_left = std::make_shared<BVHNode>(objects, start, mid);
		_right = std::make_shared<BVHNode>(objects, mid, end);
	}
}

bool BVHNode::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	if (!_bBox.Hit(r, rayT)) {
		return false;
	}

	bool hitLeft = _left->Hit(r, rayT, rec);
	bool hitRight = _right->Hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

	return hitLeft || hitRight;
}

AABB BVHNode::BoundingBox() const {
	return _bBox;
}

bool BVHNode::BoxCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b, int axisIndex) {
	auto& aAxisInterval = a->BoundingBox().AxisInterval(axisIndex);
	auto& bAxisInterval = b->BoundingBox().AxisInterval(axisIndex);
	
	return aAxisInterval.min < bAxisInterval.min;
}

bool BVHNode::BoxXCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b) {
	return BoxCompare(a, b, 0);
}

bool BVHNode::BoxYCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b) {
	return BoxCompare(a, b, 1);
}

bool BVHNode::BoxZCompare(std::shared_ptr<Hittable> const a, std::shared_ptr<Hittable> const b) {
	return BoxCompare(a, b, 2);
}