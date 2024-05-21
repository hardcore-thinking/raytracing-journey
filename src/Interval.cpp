#include "Interval.hpp"

Interval::Interval() : min(-infinity), max(+infinity) {}

Interval::Interval(double min, double max) : min(min), max(max) {}

Interval::Interval(Interval const& a, Interval const& b) {
	// Create the interval tightly enclosing the two input intervals
	min = a.min <= b.min ? a.min : b.min;
	max = a.max >= b.max ? a.max : b.max;
}

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

Interval Interval::Expand(double delta) const {
	auto padding = delta / 2;
	return Interval(min - padding, max + padding);
}

Interval const Interval::empty = Interval(+infinity, -infinity);
Interval const Interval::universe = Interval(-infinity, +infinity);