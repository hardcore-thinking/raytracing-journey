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

	// Determine where the hit point lies within the planar shape using its plane coordinates
	auto intersection = r.At(t);

	Vec3 planarHitPointVector = intersection - _q;
	auto alpha = Dot(_w, Cross(planarHitPointVector, _v));
	auto beta = Dot(_w, Cross(_u, planarHitPointVector));

	if (!IsInterior(alpha, beta, rec)) {
		return false;
	}

	// Ray hits the 2D shape; set the rest of the hit record and return true
	rec.t = t;
	rec.p = intersection;
	rec.mat = _mat;
	rec.SetFaceNormal(r, _normal);

	return true;
}

bool Quad::IsInterior(double a, double b, HitRecord& rec) const {
	Interval unitInterval = Interval(0, 1);
	// Given the hit point in plane coordinates, return false if it is outside the
	// primitive, otherwise set the hit record UV coordinates and return true

	if (!unitInterval.Contains(a) || !unitInterval.Contains(b)) {
		return false;
	}

	rec.u = a;
	rec.v = b;
	
	return true;
}