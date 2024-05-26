#include "Ray.hpp"

Ray::Ray() {}
Ray::Ray(Point3 const& origin, Vec3 const& direction) : _orig(origin), _dir(direction), _tm(0.0) {}
Ray::Ray(Point3 const& origin, Vec3 const& direction, double time) : _orig(origin), _dir(direction), _tm(time) {}

Point3 const& Ray::Origin() const {
	return _orig;
}

Vec3 const& Ray::Direction() const {
	return _dir;
}

double Ray::Time() const {
	return _tm;
}

Point3 Ray::At(double t) const {
	return _orig + t * _dir;
}