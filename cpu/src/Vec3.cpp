#include "Vec3.hpp"

Vec3::Vec3() : e{ 0, 0, 0 } {}
Vec3::Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

double Vec3::X() const {
	return e[0];
}

double Vec3::Y() const {
	return e[1];
}

double Vec3::Z() const {
	return e[2];
}

Vec3 Vec3::operator-() const { 
	return Vec3(-e[0], -e[1], -e[2]);
}

double Vec3::operator[](int i) const {
	return e[i];
}

double& Vec3::operator[](int i) {
	return e[i];
}

Vec3 Vec3::operator+=(Vec3 const& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

Vec3 Vec3::operator*=(double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

Vec3& Vec3::operator/=(double t) {
	*this *= 1 / t;
	return *this;
}

double Vec3::Length() const {
	return std::sqrt(this->LengthSquared());
}

double Vec3::LengthSquared() const {
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

Vec3 Vec3::Random() {
	return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
}

Vec3 Vec3::Random(double min, double max) {
	return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
}

bool Vec3::NearZero() const {
	auto s = 1E-8;
	return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}

// Vector utility functions
std::ostream& operator<<(std::ostream& out, Vec3 const& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

Vec3 operator+(Vec3 const& u, Vec3 const& v) {
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

Vec3 operator-(Vec3 const& u, Vec3 const& v) {
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

Vec3 operator*(Vec3 const& u, Vec3 const& v) {
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

Vec3 operator*(double t, Vec3 const& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

Vec3 operator*(Vec3 const& v, double t) {
	return t * v;
}

Vec3 operator/(Vec3 const& v, double t) {
	return (1 / t) * v;
}

double Dot(Vec3 const& u, Vec3 const& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

Vec3 Cross(Vec3 const& u, Vec3 const& v) {
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

Vec3 UnitVector(Vec3 const& v) {
	return v / v.Length();
}

Vec3 RandomInUnitSphere() {
	while (true) {
		auto p = Vec3::Random(-1, 1);
		if (p.LengthSquared() < 1) {
			return p;
		}
	}
}

Vec3 RandomUnitVector() {
	return UnitVector(RandomInUnitSphere());
}

Vec3 RandomOnHemisphere(Vec3 const& normal) {
	Vec3 onUnitSphere = RandomUnitVector();
	if (Dot(onUnitSphere, normal) > 0.0) {
		return onUnitSphere;
	}

	else {
		return -onUnitSphere;
	}
}

Vec3 Reflect(Vec3 const& v, Vec3 const& n) {
	return v - 2 * Dot(v, n) * n;
}

Vec3 Refract(Vec3 const& uv, Vec3 const& n, double etaiOverEtat) {
	auto cosTheta = std::fmin(Dot(-uv, n), 1.0);
	Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
	Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * n;
	return rOutPerp + rOutParallel;
}

Vec3 RandomInUnitDisk() {
	while (true) {
		auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.Length() < 1) {
			return p;
		}
	}
}