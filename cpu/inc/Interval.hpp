#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "Utils.hpp"

class Interval {
	public:
		double min = 0.0;
		double max = 0.0;

		Interval();
		Interval(double min, double max);
		Interval(Interval const& a, Interval const& b);

		double Size() const;
		bool Contains(double x) const;
		bool Surrounds(double x) const;
		double Clamp(double x) const;
		Interval Expand(double delta) const;

		static Interval const empty;
		static Interval const universe;
};

Interval operator+(Interval const& iVal, double displacement);
Interval operator+(double displacement, Interval const& iVal);

#endif // INTERVAL_HPP