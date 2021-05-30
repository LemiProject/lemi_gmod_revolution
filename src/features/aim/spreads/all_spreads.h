#pragma once
#include "../../../interfaces.h"
#include "../../../game_sdk/entitys/c_base_weapon.h"
#include "../../../game_sdk/interfaces/lua_shared.h"


inline void allspreads_nospread(c_base_combat_weapon* weapon, c_user_cmd* ucmd)
{
	auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
	if (!lua)
		return;
	c_lua_auto_pop ap(lua);
	
	auto spread = weapon->get_spread();
	if (spread.x == 0.f)
		return;
	auto cone = spread.x;

	if (weapon->get_weapon_base().find("cw_") != std::string::npos)
	{
		weapon->push_entity();
			lua->get_field(-1, "MaxSpreadInc");
			cone = lua->get_number();
		lua->pop(2);
	}
	
	math::lua::random_seed(ucmd->command_number);
	
	auto x = math::lua::rand(-cone, cone);
	auto y = math::lua::rand(-cone, cone);

	q_angle punch_angle;
	punch_angle = get_local_player()->get_view_punch_angles();
	
	q_angle ang;
	if (weapon->get_weapon_base().find("swb") != std::string::npos)
	{
		if (!settings::states["aim_bot::no_recoil"])
			ang = ucmd->viewangles - (punch_angle + (q_angle(x, y, 0) * 25));
		else
			ang = ucmd->viewangles - ((q_angle(x, y, 0) * 25));
	}
	else if (weapon->get_weapon_base().find("cw_") != std::string::npos)
	{
		ang = ucmd->viewangles + (q_angle(-x, -y, 0) * 25.f);
	}

	if (ang.is_valid())
		ucmd->viewangles = ang;
}
