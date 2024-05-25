#include "Hittable.hpp"

void HitRecord::SetFaceNormal(Ray const& r, Vec3 const& outwardNormal) {
	frontFace = Dot(r.Direction(), outwardNormal) < 0;
	normal = frontFace ? outwardNormal : -outwardNormal;
}

Translate::Translate(std::shared_ptr<Hittable> object, Vec3 const& offset)
	: _object(object), _offset(offset) {
	_bBox = _object->BoundingBox() + _offset;
}

bool Translate::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	// Move the ray backward by the offset
	Ray offsetR(r.Origin() - _offset, r.Direction(), r.Time());

	// Determine whether an intersection exists along the offset ray (and if so, where)
	if (!_object->Hit(offsetR, rayT, rec)) {
		return false;
	}

	// Move the intersection point forwards by the offset
	rec.p += _offset;

	return true;
}

AABB Translate::BoundingBox() const {
	return _bBox;
}