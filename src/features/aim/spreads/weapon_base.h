#pragma once
#include "../../../game_sdk/entitys/c_base_weapon.h"


inline void calc_spread_weapon_base(c_base_combat_weapon* weapon, c_user_cmd* ucmd)
{
	auto spread = weapon->get_bullet_spread();
	
	auto cone = (spread.x + spread.y + spread.z) / 3.f;

	if (weapon->is_use_lua())
		cone = weapon->get_primary_value("Spread");
	
	auto seed = md5::md5_pseudo_random(ucmd->command_number) & 0xFF;

	interfaces::random->set_seed(seed);
	
	auto x = interfaces::random->random_float(-0.5f, 0.5f)
		+ interfaces::random->random_float(-0.5f, 0.5f);
	auto y = interfaces::random->random_float(-0.5f, 0.5f)
		+ interfaces::random->random_float(-0.5f, 0.5f);;

	c_vector dir{ 1.f, cone * y, -(cone * x)};
	q_angle ang;
	math::vector_to_angels(dir, ang);

	ucmd->viewangles += ang;
}
