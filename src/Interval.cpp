#include "Interval.hpp"

Interval::Interval() : min(-infinity), max(+infinity) {}

Interval::Interval(double min, double max) : min(min), max(max) {}

double Interval::Size() const {
	return max - min;
}

bool Interval::Contains(double x) const {
	return min <= x && x <= max;
}

bool Interval::Surrounds(double x) const {
	return min < x && x < max;
}

double Interval::Clamp(double x) const {
	if (x < min) {
		return min;
	}

	if (x > max) {
		return max;
	}

	return x;
}

Interval const Interval::empty = Interval(+infinity, -infinity);
Interval const Interval::universe = Interval(-infinity, +infinity);