#pragma once
#include "../../../interfaces.h"
#include "../../../game_sdk/entitys/c_base_weapon.h"
#include "../../../game_sdk/interfaces/lua_shared.h"


inline void allspreads_nospread(c_base_combat_weapon* weapon, c_user_cmd* ucmd, std::map<std::string, float> wm)
{
	auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
	if (!lua)
		return;
	c_lua_auto_pop ap(lua);
	
	auto spread = weapon->get_spread();
	if (spread.x == 0.f)
		return;
	auto cone = spread.x;
	
	math::lua::random_seed(ucmd->command_number);
	
	auto x = math::lua::rand(-cone, cone);
	auto y = math::lua::rand(-cone, cone);

	q_angle punch_angle;

	if (wm["Recoil"] != 0)
	{
		//auto recoil = wm["Recoil"];

		//auto tmp_punch_angle = ucmd->viewangles;
		//tmp_punch_angle.x -= recoil * 0.5f;
		//tmp_punch_angle.y += math::lua::rand(-1, 1) * recoil * 0.5f;

		//tmp_punch_angle += q_angle(-recoil * 1.25f, 0, 0);

		//punch_angle = tmp_punch_angle;
		punch_angle = get_local_player()->get_view_punch_angles();
	}
	else
	{
		punch_angle = get_local_player()->get_view_punch_angles();
	}
	
	q_angle ang = ucmd->viewangles - (punch_angle + (q_angle(x, y, 0) * 25));
	
	ucmd->viewangles = ang;
}
