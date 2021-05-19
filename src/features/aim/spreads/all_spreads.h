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
	
	math::lua::random_seed(ucmd->command_number);
	
	auto x = math::lua::rand(-cone, cone);
	auto y = math::lua::rand(-cone, cone);

	auto fwd = math::get_forward(q_angle(x, y, 0) * 25.f);
	q_angle ang;
	math::vector_to_angels(fwd, ang);
	ang.normalize();
	
	ucmd->viewangles -= ang;
}
