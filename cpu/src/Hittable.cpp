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

RotateY::RotateY(std::shared_ptr<Hittable> object, double angle)
	: _object(object) {
	auto radians = DegreesToRadians(angle);
	_sinTheta = std::sin(radians);
	_cosTheta = std::cos(radians);
	_bBox = _object->BoundingBox();

	Point3 min( infinity,  infinity,  infinity);
	Point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = i * _bBox.x.max + (1 - i) * _bBox.x.min;
				auto y = j * _bBox.y.max + (1 - j) * _bBox.y.min;
				auto z = k * _bBox.z.max + (1 - k) * _bBox.z.min;

				auto newX = _cosTheta * x + _sinTheta * z;
				auto newZ = -_sinTheta * x + _cosTheta * z;
			
				Vec3 tester(newX, y, newZ);

				for (int c = 0; c < 3; c++) {
					min[c] = std::fmin(min[c], tester[c]);
					max[c] = std::fmax(max[c], tester[c]);
				}
			}
		}
	}

	_bBox = AABB(min, max);
}

bool RotateY::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	// Change the ray from world space to object space
	auto origin = r.Origin();
	auto direction = r.Direction();

	origin[0] = _cosTheta * r.Origin()[0] - _sinTheta * r.Origin()[2];
	origin[2] = _sinTheta * r.Origin()[0] + _cosTheta * r.Origin()[2];

	direction[0] = _cosTheta * r.Direction()[0] - _sinTheta * r.Direction()[2];
	direction[2] = _sinTheta * r.Direction()[0] + _cosTheta * r.Direction()[2];

	Ray rotatedR(origin, direction, r.Time());

	// Determine whether an intersection exists in object space (and if so, where)
	if (!_object->Hit(rotatedR, rayT, rec)) {
		return false;
	}

	// Chnage the intersection point from object space to world space
	auto p = rec.p;
	p[0] = _cosTheta * rec.p[0] + _sinTheta * rec.p[2];
	p[2] = -_sinTheta * rec.p[0] + _cosTheta * rec.p[2];

	// Change the normal from object space to world space
	auto normal = rec.normal;
	normal[0] = _cosTheta * rec.normal[0] + _sinTheta * rec.normal[2];
	normal[2] = -_sinTheta * rec.normal[0] + _cosTheta * rec.normal[2];

	rec.p = p;
	rec.normal = normal;

	return true;
}

AABB RotateY::BoundingBox() const {
	return _bBox;
}