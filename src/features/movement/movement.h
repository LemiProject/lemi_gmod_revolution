#pragma once
#include "../../game_sdk/misc/c_user_cmd.h"
#include "../../game_sdk/misc/c_view_setup.h"

namespace movement
{
	namespace bunny_hop
	{
		void run_bunny_hop(c_user_cmd& ucmd);
		void run_auto_strafe(c_user_cmd& cmd);
	}

	namespace free_cam
	{
		inline auto is_in_free_cam = false;

		void run_free_cam(c_view_setup& setup, c_vector& camera_position);
	}
	
	__forceinline void run_movement(c_user_cmd& ucmd)
	{
		bunny_hop::run_bunny_hop(ucmd);
		bunny_hop::run_auto_strafe(ucmd);
	}
}
