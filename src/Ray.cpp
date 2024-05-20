#include "Ray.hpp"

Ray::Ray() {}
Ray::Ray(Point3 const& origin, Vec3 const& direction) : orig(origin), dir(direction) {}

Point3 const& Ray::Origin() const {
	return orig;
}

Vec3 const& Ray::Direction() const {
	return dir;
}

Point3 Ray::At(double t) const {
	return orig + t * dir;
}