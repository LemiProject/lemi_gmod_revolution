#pragma once
#include "vector.h"
#include <imgui/im_tools.h>

namespace math
{
	
	struct c_box
	{
		float x, y, w, h;

		ImVec2 get_min() const { return { x, y }; }
		ImVec2 get_max() const { return { x + w, y + h }; }
	};
	
	
}
