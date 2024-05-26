#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <vector>
#include <array>

#include "Vec3.hpp"
#include "Utils.hpp"

using TrilinearInterpolationCube = std::array<Vec3, 8>;

class Perlin {
	public:
		Perlin();
		~Perlin();

		double Noise(Point3 const& p) const;
		double Turbulence(Point3 const& p, int depth) const;

	private:
		static int const    _pointCount = 256;
		std::vector<double> _randFloat  = {};
		std::vector<Vec3>   _randVec    = {};
		std::vector<int>    _permX      = {};
		std::vector<int>    _permY      = {};
		std::vector<int>    _permZ      = {};

		static std::vector<int> PerlinGeneratePerm();
		static void Permute(std::vector<int>& p, int n);
		static double PerlinInterpolation(TrilinearInterpolationCube c, double u, double v, double w);
};

#endif // PERLIN_HPP