#include "Hittable.hpp"

void HitRecord::SetFaceNormal(Ray const& r, Vec3 const& outwardNormal) {
	frontFace = Dot(r.Direction(), outwardNormal) < 0;
	normal = frontFace ? outwardNormal : -outwardNormal;
}