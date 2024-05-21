#include "AABB.hpp"

AABB::AABB() {}

AABB::AABB(Interval const& x, Interval const& y, Interval const& z) : x(x), y(y), z(z) {}

AABB::AABB(Point3 const& a, Point3 const& b) {
	// Treat the two points a anb b as extrema for the bounding box, so we don't require a
	// particular minimun/maximum coordinate order

	x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
	y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
	z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
}

Interval const& AABB::AxisInterval(int n) const {
	if (n == 1) {
		return y;
	}

	if (n == 2) {
		return z;
	}

	return x;
}

bool AABB::Hit(Ray const& r, Interval rayT) const {
	Point3 const& rayOrig = r.Origin();
	Vec3 const& rayDir    = r.Direction();

	for (int axis = 0; axis < 3; axis++) {
		Interval const& ax = AxisInterval(axis);
		double const adInv = 1.0 / rayDir[axis];

		auto t0 = (ax.min - rayOrig[axis]) * adInv;
		auto t1 = (ax.max - rayOrig[axis]) * adInv;

		if (t0 < t1) {
			if (t0 > rayT.min) {
				rayT.min = t0;
			}

			if (t1 < rayT.max) {
				rayT.max = t1;
			}
		}

		else {
			if (t1 > rayT.min) {
				rayT.min = t1;
			}

			if (t0 < rayT.max) {
				rayT.max = t0;
			}
		}

		if (rayT.max <= rayT.min) {
			return false;
		}

		return true;
	}
}