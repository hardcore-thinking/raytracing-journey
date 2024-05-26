#include "ConstantMedium.hpp"

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> tex)
	: _boundary(boundary), _negInvDensity(-1 / density), _phaseFunction(std::make_shared<Isotropic>(tex)) {}

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, double density, Color const& albedo)
	: _boundary(boundary), _negInvDensity(-1 / density), _phaseFunction(std::make_shared<Isotropic>(albedo)) {}

bool ConstantMedium::Hit(Ray const& r, Interval rayT, HitRecord& rec) const {
	// Print occasional sampkes when debugging. To enable set enableDebug true.
	bool const enableDebug = false;
	bool const debugging = enableDebug && RandomDouble() < 0.00001;

	HitRecord rec1 = {};
	HitRecord rec2 = {};

	if (!_boundary->Hit(r, Interval::universe, rec1)) {
		return false;
	}

	if (!_boundary->Hit(r, Interval(rec1.t + 0.0001, infinity), rec2)) {
		return false;
	}

	if (debugging) {
		std::clog << "\ntMin = " << rec1.t << ", tMax = " << rec2.t << '\n';
	}

	if (rec1.t < rayT.min) {
		rec1.t = rayT.min;
	}

	if (rec2.t > rayT.max) {
		rec2.t = rayT.max;
	}

	if (rec1.t >= rec2.t) {
		return false;
	}

	if (rec1.t < 0) {
		rec1.t = 0;
	}

	auto rayLength = r.Direction().Length();
	auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
	auto hitDistance = _negInvDensity * std::log(RandomDouble());

	if (hitDistance > distanceInsideBoundary) {
		return false;
	}

	rec.t = rec1.t + hitDistance / rayLength;
	rec.p = r.At(rec.t);

	if (debugging) {
		std::clog << "hitDistance = " << hitDistance << '\n'
				  << "rec.t = " << rec.t << '\n'
				  << "rec.p = " << rec.p << '\n';
	}

	rec.normal = Vec3(1, 0, 0); // arbitrary
	rec.frontFace = true; // also arbitrary
	rec.mat = _phaseFunction;

	return true;
}

AABB ConstantMedium::BoundingBox() const {
	return _boundary->BoundingBox();
}