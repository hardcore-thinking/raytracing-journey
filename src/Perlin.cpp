#include "Perlin.hpp"

Perlin::Perlin() {
	_randVec.resize(_pointCount);

	for (auto& vec : _randVec) {
		vec = UnitVector(Vec3::Random(-1, 1));
	}

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
	auto u = p.X() - floor(p.X());
	auto v = p.Y() - floor(p.Y());
	auto w = p.Z() - floor(p.Z());

	auto i = static_cast<int>(floor(p.X()));
	auto j = static_cast<int>(floor(p.Y()));
	auto k = static_cast<int>(floor(p.Z()));
	TrilinearInterpolationCube c;
	c.fill(Vec3());

	for (int di = 0; di < 2; di++) {
		for (int dj = 0; dj < 2; dj++) {
			for (int dk = 0; dk < 2; dk++) {
				c[di * 4 + dj * 2 + dk] = _randVec[
					_permX[(i + di) & 255] ^
					_permY[(j + dj) & 255] ^
					_permZ[(k + dk) & 255]
				];
			}
		}
	}

	return PerlinInterpolation(c, u, v, w);
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

double Perlin::PerlinInterpolation(TrilinearInterpolationCube c, double u, double v, double w) {
	auto uu = u * u * (3 - 2 * u);
	auto vv = v * v * (3 - 2 * v);
	auto ww = w * w * (3 - 2 * w);
	auto accum = 0.0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				Vec3 weightV(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu))
					   * (j * vv + (1 - j) * (1 - vv))
					   * (k * ww + (1 - k) * (1 - ww))
					   * Dot(c[i * 4 + j * 2 + k], weightV);
			}
		}
	}

	return accum;
}