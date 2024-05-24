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
	auto u = p.X() - floor(p.X());
	auto v = p.Y() - floor(p.Y());
	auto w = p.Z() - floor(p.Z());

	u = u * u * (3 - 2 * u);
	v = v * v * (3 - 2 * v);
	w = w * w * (3 - 2 * w);

	auto i = static_cast<int>(floor(p.X()));
	auto j = static_cast<int>(floor(p.Y()));
	auto k = static_cast<int>(floor(p.Z()));
	TrilinearInterpolationCube c;
	c.fill(0.0);

	for (int di = 0; di < 2; di++) {
		for (int dj = 0; dj < 2; dj++) {
			for (int dk = 0; dk < 2; dk++) {
				c[di * 4 + dj * 2 + dk] = _randFloat[
					_permX[(i + di) & 255] ^
					_permY[(j + dj) & 255] ^
					_permZ[(k + dk) & 255]
				];
			}
		}
	}

	return TrilinearInterpolation(c, u, v, w);
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

double Perlin::TrilinearInterpolation(TrilinearInterpolationCube c, double u, double v, double w) {
	auto accum = 0.0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				accum += (i * u + (1 - i) * (1 - u))
					   * (j * v + (1 - j) * (1 - v))
					   * (k * w + (1 - k) * (1 - w))
					   * c[i * 4 + j * 2 + k];
			}
		}
	}

	return accum;
}