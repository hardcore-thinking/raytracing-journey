#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

class Ray {
	public:
		Ray();
		Ray(Point3 const& origin, Vec3 const& direction);
		Ray(Point3 const& origin, Vec3 const& direction, double time);

		Point3 const& Origin() const;
		Vec3 const& Direction() const;

		double Time() const;

		Point3 At(double t) const;

	private:
		Point3 _orig = {};
		Vec3 _dir = {};
		double _tm = 0.0;
};

#endif // RAY_HPP
