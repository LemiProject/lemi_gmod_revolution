#pragma once
#include "../../game_sdk/misc/c_user_cmd.h"


namespace movement
{
	namespace bunny_hop
	{
		void run_bunny_hop(c_user_cmd& ucmd);
	}
	
	__forceinline void run_movement(c_user_cmd& ucmd)
	{
		bunny_hop::run_bunny_hop(ucmd);
	}
}
