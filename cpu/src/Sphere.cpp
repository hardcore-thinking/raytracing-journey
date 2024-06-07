#include "Sphere.hpp"

Sphere::Sphere(Point3 const& center, double radius, std::shared_ptr<Material> mat)
	: _center1(center), _radius(std::fmax(0, radius)), _mat(mat), _isMoving(false) {
	auto rVec = Vec3(radius, radius, radius);
	_bBox = AABB(_center1 - rVec, _center1 + rVec);
}

Sphere::Sphere(Point3 const& center1, Point3 const& center2, double radius, std::shared_ptr<Material> mat) 
	: _center1(center1), _radius(std::fmax(0, radius)), _mat(mat), _isMoving(true) {
	auto rVec = Vec3(radius, radius, radius);
	AABB box1(_center1 - rVec, _center1 + rVec);
	AABB box2(center2 - rVec, center2 + rVec);
	_bBox = AABB(box1, box2);

	_centerVec = center2 - center1;
}

bool Sphere::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	Point3 center = _isMoving ? SphereCenter(r.Time()) : _center1;

	Vec3 oc = center - r.Origin();

	auto a = r.Direction().LengthSquared();
	auto h = Dot(r.Direction(), oc);
	auto c = oc.LengthSquared() - _radius * _radius;

	auto discriminant = h * h - a * c;

	if (discriminant < 0) {
		return false;
	}

	auto sqrtd = std::sqrt(discriminant);

	auto root = (h - sqrtd) / a;
	if (!rayT.Surrounds(root)) {
		root = (h + sqrtd) / a;
		if (!rayT.Surrounds(root)) {
			return false;
		}
	}

	rec.t = root;
	rec.p = r.At(rec.t);
	Vec3 outwardNormal = (rec.p - _center1) / _radius;
	rec.SetFaceNormal(r, outwardNormal);
	GetSphereUV(outwardNormal, rec.u, rec.v);
	rec.mat = _mat;

	return true;
}

AABB Sphere::BoundingBox() const {
	return _bBox;
}

Point3 Sphere::SphereCenter(double time) const {
	// Linear interpolate from center1 to center2 according to time, where t=0 yields
	// center1, and t=1 yields center2
	return _center1 + time * _centerVec;
}

void Sphere::GetSphereUV(Point3 const& p, double& u, double& v) {
	// p: a given point on the sphere of radius one, centered at the origin
	// u: returned value [0, 1] of angle around the Y axis from X = -1
	// v: returned value [0, 1] of angle from Y = -1 to Y = +1
	//     <1 0 0> yiels <0.50 0.50>        <-1   0   0> yields <0.00 0.50>
	//     <0 1 0> yiels <0.50 1.00>        < 0  -1   0> yields <0.50 0.00>
	//     <0 0 1> yiels <0.25 0.50>        < 0   0  -1> yields <0.75 0.50>

	auto theta = std::acos(-p.Y());
	auto phi = std::atan2(-p.Z(), p.X()) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}