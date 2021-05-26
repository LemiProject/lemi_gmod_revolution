#pragma once
#include <cfloat>
#include <cmath>


class c_vector
{
public:
	float x, y, z;

	c_vector();
	c_vector(const c_vector&& left) noexcept;
	c_vector(const c_vector& left);
	c_vector(c_vector& left);
	c_vector(float _x, float _y, float _z);
	c_vector(float* ptr);
	c_vector(float val);
	
	void init(float _x, float _y, float _z);

	void make_inf();
	void make_zero();

	void normalize();
	void clamp();
	
	float length() const;
	float length2d() const;
	float distance_to(const c_vector& to) const;
	
	[[nodiscard]] bool is_valid() const;

	[[nodiscard]] float* base();
	[[nodiscard]] float const* base() const;

	float operator[] (int idx) const;
	float& operator[] (int idx);

	c_vector& operator=(const c_vector& left);
	bool operator==(const c_vector& other) const;

	c_vector operator+(const c_vector& left) const;
	c_vector operator-(const c_vector& left) const;
	c_vector operator*(const c_vector& left) const;
	c_vector operator/(const c_vector& left) const;

	c_vector& operator+=(const c_vector& left);
	c_vector& operator-=(const c_vector& left);
	c_vector& operator*=(const c_vector& left);
	c_vector& operator/=(const c_vector& left);

	c_vector to_string_friendly() const
	{
		if (!is_valid())
			return { 0.f };

		auto out = *this;
		
		if (out.x > FLT_MAX)
			out.x = FLT_MAX;
		if (out.y > FLT_MAX)
			out.y = FLT_MAX;
		if (out.z > FLT_MAX)
			out.z = FLT_MAX;

		out.x = roundf(out.x);
		out.y = roundf(out.y);
		out.z = roundf(out.z);
		
		return out;
	}
};

class __declspec(align(16)) c_vector_aligned : public c_vector {
public:
	inline c_vector_aligned(void) {};
	inline c_vector_aligned(float X, float Y, float Z) {
		init(X, Y, Z);
	}

public:
	explicit c_vector_aligned(const c_vector& vOther) {
		init(vOther.x, vOther.y, vOther.z);
	}

	c_vector_aligned& operator=(const c_vector& vOther) {
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};