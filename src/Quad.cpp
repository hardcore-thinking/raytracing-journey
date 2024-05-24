#include "Quad.hpp"

Quad::Quad(Point3 const& q, Vec3 const& u, Vec3 const& v, std::shared_ptr<Material> mat)
	: _q(q), _u(u), _v(v), _mat(mat) {
	auto n = Cross(u, v);
	_normal = UnitVector(n);
	_d = Dot(_normal, _q);
	_w = n / Dot(n, n);

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
	auto denom = Dot(_normal, r.Direction());

	// No hit if the ray is parallel to the plane
	if (std::fabs(denom) < 1E-8) {
		return false;
	}

	// Return false if the hit point parameter t is outside the ray interval
	auto t = (_d - Dot(_normal, r.Origin())) / denom;
	if (!rayT.Contains(t)) {
		return false;
	}

	auto intersection = r.At(t);

	rec.t = t;
	rec.p = intersection;
	rec.mat = _mat;
	rec.SetFaceNormal(r, _normal);

	return true;
}