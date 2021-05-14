#include "vector.h"
#include <limits>
#include <cmath>
#include <limits>
#include <algorithm>

c_vector::c_vector()
{
	make_inf();
}

c_vector::c_vector(const c_vector&& left) noexcept : x(left.x), y(left.y), z(left.z)
{
	
}

c_vector::c_vector(const c_vector& left) : x(left.x), y(left.y), z(left.z)
{
	
}

c_vector::c_vector(c_vector& left) : x(left.x), y(left.y), z(left.z)
{
}

c_vector::c_vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

c_vector::c_vector(float* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2])
{
}

c_vector::c_vector(float val) : x(val), y(val), z(val)
{
	
}

void c_vector::init(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

void c_vector::make_inf()
{
	x = y = z = std::numeric_limits<float>::infinity();
}

void c_vector::make_zero()
{
	x = y = z = 0.f;
}

void c_vector::normalize()
{
	while (x > 89.0f)
		x -= 180.0f;
	while (x < -89.0f)
		x += 180.0f;
	while (y < -180.0f)
		y += 360.0f;
	while (y > 180.0f)
		y -= 360.0f;
	z = 0.f;
}

void c_vector::clamp()
{
	while (this->x < -180.0f)
		this->x += 360.0f;
	while (this->x > 180.0f)
		this->x -= 360.0f;
	if (this->x > 89.0f)
		this->x = 89.0f;
	if (this->x < -89.0f)
		this->x = -89.0f;
	while (this->y < -180.0f)
		this->y += 360.0f;
	while (this->y > 180.0f)
		this->y -= 360.0f;

	this->z = 0.0f;
}

float c_vector::length() const
{
	return sqrt(x * x + y * y + z * z);
}

float c_vector::length2d() const
{
	return sqrt(x * x + y * y);
}

float c_vector::distance_to(const c_vector& to) const
{
	return (*this - to).length();
}

bool c_vector::is_valid() const
{
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

float* c_vector::base()
{
	return reinterpret_cast<float*>(this);
}

float const* c_vector::base() const
{
	return reinterpret_cast<float const*>(this);
}

float c_vector::operator[](int idx) const
{
	return ((float*)this)[idx];
}

float& c_vector::operator[](int idx)
{
	return ((float*)this)[idx];
}

c_vector& c_vector::operator=(const c_vector& left)
= default;

bool c_vector::operator==(const c_vector& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

c_vector c_vector::operator+(const c_vector& left) const
{
	return { x + left.x, y + left.y, z + left.z };
}

c_vector c_vector::operator-(const c_vector& left) const
{
	return { x - left.x, y - left.y, z - left.z };
}

c_vector c_vector::operator*(const c_vector& left) const
{
	return { x * left.x, y * left.y, z * left.z };
}

c_vector c_vector::operator/(const c_vector& left) const
{
	return { x / left.x, y / left.y, z / left.z };
}


c_vector& c_vector::operator+=(const c_vector& left)
{
	x += left.x;
	y += left.y;
	z += left.z;
	return *this;
}

c_vector& c_vector::operator-=(const c_vector& left)
{
	x -= left.x;
	y -= left.y;
	z -= left.z;
	return *this;
}

c_vector& c_vector::operator*=(const c_vector& left)
{
	x *= left.x;
	y *= left.y;
	z *= left.z;
	return *this;
}

c_vector& c_vector::operator/=(const c_vector& left)
{
	x /= left.x;
	y /= left.y;
	z /= left.z;
	return *this;
}
