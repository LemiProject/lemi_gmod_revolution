#pragma once
#include "vector.h"
#include <imgui/im_tools.h>

namespace math
{
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
}
