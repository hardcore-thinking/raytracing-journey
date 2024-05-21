#ifndef AABB_HPP
#define AABB_HPP

#include "Interval.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

class AABB {
public:
	Interval x = {};
	Interval y = {};
	Interval z = {};

	AABB();
	AABB(Interval const& x, Interval const& y, Interval const& z);
	AABB(Point3 const& a, Point3 const& b);
	AABB(AABB const& box0, AABB const& box1);

	Interval const& AxisInterval(int n) const;
	bool Hit(Ray const& r, Interval rayT) const;

	int LongestAxis() const;

	static AABB const empty;
	static AABB const universe;
};

#endif // AABB_HPP