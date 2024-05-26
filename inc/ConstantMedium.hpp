#ifndef CONSTANTMEDIUM_HPP
#define CONSTANTMEDIUM_HPP

#include <iostream>
#include <memory>

#include "Hittable.hpp"
#include "Material.hpp"

class ConstantMedium : public Hittable {
	public:
		ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> tex);
		ConstantMedium(std::shared_ptr<Hittable> boundary, double density, Color const& albedo);

		bool Hit(Ray const& r, Interval rayT, HitRecord& rec) const override;
		AABB BoundingBox() const override;
		
	private:
		std::shared_ptr<Hittable> _boundary = nullptr;
		double _negInvDensity = 0.0;
		std::shared_ptr<Material> _phaseFunction = nullptr;
};

#endif // CONSTANTMEDIUM_HPP