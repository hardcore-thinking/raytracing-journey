#include "Quad.hpp"

Quad::Quad(Point3 const& q, Vec3 const& u, Vec3 const& v, std::shared_ptr<Material> mat)
	: _q(q), _u(u), _v(v), _mat(mat) {
	SetBoundingBox();
}

void Quad::SetBoundingBox() {
	// Compute the bounding box of all four vertices
	auto bBoxDiagonal1 = AABB(_q, _q + _u + _v);
	auto bBoxDiagonal2 = AABB(_q + _u, _q +_v);
	_bBox = AABB(bBoxDiagonal1, bBoxDiagonal2);
}

AABB Quad::BoundingBox() const {
	return _bBox;
}

bool Quad::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	return false; // To be implemented
}