#include "Perlin.hpp"

Perlin::Perlin() {
	_randFloat = std::vector<double>(_pointCount);
	for (auto& rand : _randFloat) {
		rand = RandomDouble();
	}

	_permX = PerlinGeneratePerm();
	_permY = PerlinGeneratePerm();
	_permZ = PerlinGeneratePerm();
}

Perlin::~Perlin() {}

double Perlin::Noise(Point3 const& p) const {
	auto i = static_cast<int>(4 * p.X()) & 255;
	auto j = static_cast<int>(4 * p.Y()) & 255;
	auto k = static_cast<int>(4 * p.Z()) & 255;

	return _randFloat[_permX[i] ^ _permY[j] ^ _permZ[k]];
}

std::vector<int> Perlin::PerlinGeneratePerm() {
	auto p = std::vector<int>(_pointCount);

	for (int i = 0; i < _pointCount; i++) {
		p[i] = i;
	}

	Permute(p, _pointCount);

	return p;
}

void Perlin::Permute(std::vector<int>& p, int n) {
	for (int i = n - 1; i > 0; i--) {
		int target = RandomInt(0, i);
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}
