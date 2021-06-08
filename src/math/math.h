#pragma once
#include "vector.h"
#include <imgui/im_tools.h>

namespace math
{
	constexpr int pitch = 0;
	constexpr int yaw = 1;
	constexpr int roll = 2;
	
	struct vec2_t
	{
		float x, y;

		ImVec2 get_im_vec2() const
		{
			return { x, y };
		}
	};

	struct box_t
	{
		float x, y, w, h;

		vec2_t get_min() const { return { x, y }; }
		vec2_t get_max() const { return { x + w, y + h }; }
	};

	__forceinline box_t create_box(vec2_t min, vec2_t max)
	{
		return { min.x, min.y, max.x - min.x, max.y - min.y };
	}

	constexpr float PI_F = 3.14159265358979323846f;
	constexpr double PI_D = 3.14159265358979323846;
	
	float rad2deg(float x);
	double rad2deg(double x);

	float deg2rad(float x);
	double deg2rad(double x);

	float procent(float var, float full);
	double procent(double var, double full);

	float float_negative(float var);
	double double_negative(double var);

	__forceinline void sincos(float x, float* s, float* c)
	{
		*s = sin(x);
		*c = cos(x);
	}
	
	namespace lua
	{ //TODO: IMPL THIS AND CREATE CW2 NOSPREAD
		double rand(double min, double max);
		void random_seed(double seed);
		
	}
}

using q_angle = c_vector;

namespace math
{
	inline void vector_to_angel(const c_vector& forward, q_angle& out)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / PI_F);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / PI_F);
			if (pitch < 0)
				pitch += 360;
		}

		out[0] = pitch;
		out[1] = yaw;
		out[2] = 0;
	}

	inline float normalize_angle(float a)
	{
		a = fmodf(a, 360.f);
		if (a > 180.f)
			a -= 360.f;
		if (a < -180.f)
			a += 360.f;
		return a;
	}

	inline q_angle fix_angles(q_angle angels)
	{
		angels.x = normalize_angle(angels.x);
		angels.y = normalize_angle(angels.y);
		if (angels.x > 89.f)
			angels.x = 89.f;
		else if (angels.x < -89.f)
			angels.x = -89.f;
		return angels;
	}

	inline q_angle get_angle(q_angle src, q_angle dst)
	{
		q_angle out_angle;
		vector_to_angel(dst - src, out_angle);
		return out_angle;
	}

	inline c_vector get_forward(q_angle ang)
	{
		ang = fix_angles(ang);
		const auto radx = deg2rad(ang.x);
		const auto rady = deg2rad(ang.y);
		return c_vector(cos(radx) * cos(rady), cos(radx) * sin(rady), sin(radx));
	}

	inline void angle_to_vector(const q_angle& ang, c_vector& vector)
	{
		float sp, sy, cp, cy;
		sincos(deg2rad(ang[yaw]), &sy, &cy);
		sincos(deg2rad(ang[pitch]), &sp, &cp);
		
		vector = {cp * cy, cp * sy, -sp};
	}

	inline void angle_to_vectors(const q_angle& ang, c_vector& forward, c_vector& right, c_vector& up)
	{
		float sr, sp, sy, cr, cp, cy;
		float radx = math::deg2rad(ang.x);
		float rady = math::deg2rad(ang.y);
		float radz = math::deg2rad(ang.z);
		sp = sin(radx); cp = cos(radx);
		sy = sin(rady); cy = cos(rady);
		sr = sin(radz); cr = cos(radz);

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = -1 * sr * cp;

		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = cr * cp;
	}
}



struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_fl_mat_val[0][0] = m00;	m_fl_mat_val[0][1] = m01; m_fl_mat_val[0][2] = m02; m_fl_mat_val[0][3] = m03;
		m_fl_mat_val[1][0] = m10;	m_fl_mat_val[1][1] = m11; m_fl_mat_val[1][2] = m12; m_fl_mat_val[1][3] = m13;
		m_fl_mat_val[2][0] = m20;	m_fl_mat_val[2][1] = m21; m_fl_mat_val[2][2] = m22; m_fl_mat_val[2][3] = m23;
	}

	void init(const c_vector& xAxis, const c_vector& yAxis, const c_vector& zAxis, const c_vector& vecOrigin)
	{
		m_fl_mat_val[0][0] = xAxis.x; m_fl_mat_val[0][1] = yAxis.x; m_fl_mat_val[0][2] = zAxis.x; m_fl_mat_val[0][3] = vecOrigin.x;
		m_fl_mat_val[1][0] = xAxis.y; m_fl_mat_val[1][1] = yAxis.y; m_fl_mat_val[1][2] = zAxis.y; m_fl_mat_val[1][3] = vecOrigin.y;
		m_fl_mat_val[2][0] = xAxis.z; m_fl_mat_val[2][1] = yAxis.z; m_fl_mat_val[2][2] = zAxis.z; m_fl_mat_val[2][3] = vecOrigin.z;
	}

	matrix3x4_t(const c_vector& xAxis, const c_vector& yAxis, const c_vector& zAxis, const c_vector& vecOrigin)
	{
		init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float _i = (unsigned long)0x7FC00000;
				float val = *reinterpret_cast<float*>(&_i);
				m_fl_mat_val[i][j] = val;
			}
		}
	}

	float* operator[](int i) { return m_fl_mat_val[i]; }
	const float* operator[](int i) const { return m_fl_mat_val[i]; }
	float* base() { return &m_fl_mat_val[0][0]; }
	const float* base() const { return &m_fl_mat_val[0][0]; }

	float m_fl_mat_val[3][4];
};