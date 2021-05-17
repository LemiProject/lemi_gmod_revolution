#pragma once
#include "../../../interfaces.h"
#include "../../../game_sdk/entitys/c_base_weapon.h"
#include "../../../game_sdk/interfaces/lua_shared.h"


inline void cw2_nospread(c_base_combat_weapon* weapon, c_user_cmd* ucmd)
{
	auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
	if (!lua)
		return;

	weapon->push_entity(); //1
	lua->get_field(-1, "MaxSpreadInc"); //2
	auto spread = lua->get_number();
	lua->pop(2);

	
}
