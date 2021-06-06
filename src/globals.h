#pragma once
#include "game_sdk/misc/c_view_setup.h"
#include "math/math.h"
#include "game_sdk/misc/c_user_cmd.h"

namespace globals
{
	namespace view
	{
		inline c_view_setup last_view_setup;
	}

	namespace local_player_states
	{
		inline c_vector velocity;
		inline q_angle viewangles;
		inline c_vector position;
		inline c_vector real_eye_pos;
		
		inline bool is_fake_duck;
		inline bool in_third_person;
		
#ifdef _DEBUG
		inline c_vector land_position;
#endif

	}

	namespace game_data
	{
		inline float ping;

		inline c_user_cmd last_cmd;
	}
}
