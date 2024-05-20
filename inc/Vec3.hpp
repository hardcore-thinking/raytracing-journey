#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>
#include <cmath>

#include "Utils.hpp"

class Vec3 {
	public:
		double e[3] = { 0.0, 0.0, 0.0 };

		Vec3();
		Vec3(double e0, double e1, double e2);

		double X() const;
		double Y() const;
		double Z() const;

		Vec3 operator-() const;
		double operator[](int i) const;
		double& operator[](int i);
		Vec3 operator+=(Vec3 const& v);
		Vec3 operator*=(double t);
		Vec3& operator/=(double t);

		double Length() const;
		double LengthSquared() const;
		static Vec3 Random();
		static Vec3 Random(double min, double max);
		bool NearZero() const;
};

// Point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

// Vector utility functions
std::ostream& operator<<(std::ostream& out, Vec3 const& v);
Vec3 operator+(Vec3 const& u, Vec3 const& v);
Vec3 operator-(Vec3 const& u, Vec3 const& v);
Vec3 operator*(Vec3 const& u, Vec3 const& v);
Vec3 operator*(double t, Vec3 const& v);
Vec3 operator*(Vec3 const& v, double t);
Vec3 operator/(Vec3 const& v, double t);
double Dot(Vec3 const& u, Vec3 const& v);
Vec3 Cross(Vec3 const& u, Vec3 const& v);
Vec3 UnitVector(Vec3 const& v);
Vec3 RandomInUnitSphere();
Vec3 RandomUnitVector();
Vec3 RandomOnHemisphere(Vec3 const& normal);
Vec3 Reflect(Vec3 const& v, Vec3 const& n);
Vec3 Refract(Vec3 const& uv, Vec3 const& n, double etaiOverEtat);
Vec3 RandomInUnitDisk();

#endif // VEC3_HPP
