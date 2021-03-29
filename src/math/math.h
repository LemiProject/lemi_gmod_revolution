#pragma once
#include "vector.h"
#include <imgui/im_tools.h>

namespace math
{
	struct vec2_t
	{
		float x, y;
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
	
}
