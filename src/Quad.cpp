#include "Quad.hpp"

Quad::Quad(Point3 const& q, Vec3 const& u, Vec3 const& v, std::shared_ptr<Material> mat, bool transparentBackFace)
	: _q(q), _u(u), _v(v), _mat(mat), _transparentBackFace(transparentBackFace) {
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

	// Hit on the back face
	if (_transparentBackFace && denom > 0) {
		return false;
	}

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

	if (_transparentBackFace && Dot(_normal, r.Direction()) > 0) {
		return false;
	}

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

std::shared_ptr<HittableList> Box(Point3 const& a, Point3 const& b, std::shared_ptr<Material> mat) {
	// Returns the 3D box (six sides) that contains the two opposite vertices a and b

	auto sides = std::make_shared<HittableList>();

	// Construct the tow opposite vertices with the minimum and maximum coordinates
	auto min = Point3(std::fmin(a.X(), b.X()), std::fmin(a.Y(), b.Y()), std::fmin(a.Z(), b.Z()));
	auto max = Point3(std::fmax(a.X(), b.X()), std::fmax(a.Y(), b.Y()), std::fmax(a.Z(), b.Z()));

	auto dx = Vec3(max.X() - min.X(), 0, 0);
	auto dy = Vec3(0, max.Y() - min.Y(), 0);
	auto dz = Vec3(0, 0, max.Z() - min.Z());

	sides->Add(std::make_shared<Quad>(Point3(min.X(), min.Y(), max.Z()),  dx,  dy, mat)); // front
	sides->Add(std::make_shared<Quad>(Point3(max.X(), min.Y(), max.Z()), -dz,  dy, mat)); // right
	sides->Add(std::make_shared<Quad>(Point3(max.X(), min.Y(), min.Z()), -dx,  dy, mat)); // back
	sides->Add(std::make_shared<Quad>(Point3(min.X(), min.Y(), min.Z()),  dz,  dy, mat)); // left
	sides->Add(std::make_shared<Quad>(Point3(min.X(), max.Y(), max.Z()),  dx, -dz, mat)); // top
	sides->Add(std::make_shared<Quad>(Point3(min.X(), min.Y(), min.Z()),  dx,  dz, mat)); // bottom

	return sides;
}