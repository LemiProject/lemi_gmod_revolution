#pragma once
#include "../../game_sdk/misc/c_user_cmd.h"


namespace aim
{
	void legit_bot(c_user_cmd* cmd);
	void anti_recoil_and_spread(c_user_cmd* ucmd);
	
	__forceinline void run_aimbot(c_user_cmd* cmd)
	{
		legit_bot(cmd);
	}
}
