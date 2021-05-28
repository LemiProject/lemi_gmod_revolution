#pragma once
#include <string>

#include "../../../game_sdk/entitys/c_base_weapon.h"


inline void fas2nospread(c_base_combat_weapon* weapon, c_user_cmd* ucmd, std::map<std::string, float> wm)
{
	auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
	if (!lua)
		return;
	c_lua_auto_pop ap(lua);

	float cone;
	
	{
		c_lua_auto_pop ap2(lua);
		weapon->push_entity();
		lua->get_field(-1, "CurCone");
		cone = lua->get_number();
	}

	if (cone == 0.f)
		return;

	math::lua::random_seed(get_local_player()->get_tick_base() * interfaces::global_vars->interval_per_tick);
	float x = math::lua::rand(-cone, cone);
	float y = math::lua::rand(-cone, cone);

	auto ang = q_angle(-x, -y, 0) * 25.f;

	ucmd->viewangles += ang;
}