#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

class Ray {
public:
	Ray();
	Ray(Point3 const& origin, Vec3 const& direction);

	Point3 const& Origin() const;
	Vec3 const& Direction() const;

	Point3 At(double t) const;

private:
	Point3 orig = {};
	Vec3 dir = {};
};

#endif // RAY_HPP
