#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "Utils.hpp"

class Interval {
	public:
		double min = 0.0;
		double max = 0.0;

		Interval();
		Interval(double min, double max);

		double Size() const;
		bool Contains(double x) const;
		bool Surrounds(double x) const;
		double Clamp(double x) const;

		static Interval const empty;
		static Interval const universe;

};

#endif // INTERVAL_HPP