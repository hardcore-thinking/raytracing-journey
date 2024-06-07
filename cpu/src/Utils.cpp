#include "Utils.hpp"

double DegreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

double RandomDouble() {
	static std::random_device randomDevice = {};
	static std::mt19937 generator(randomDevice());

	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	return distribution(generator);
}

double RandomDouble(double min, double max) {
	return min + (max - min) * RandomDouble();
}

int RandomInt(int min, int max) {
	return static_cast<int>(RandomDouble(min, max + 1));
}
