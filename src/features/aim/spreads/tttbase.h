#pragma once
#include "../../../interfaces.h"
#include "../../../game_sdk/entitys/c_base_weapon.h"
#include "../../../math/math.h"

inline void calc_spread_ttt(c_base_combat_weapon* weapon, c_user_cmd* ucmd)
{
	const auto spread_cone = q_angle(weapon->get_primary_value("Cone"));
	if (spread_cone == q_angle(-1.f))
		return;
	
	const auto spread = -((spread_cone.x + spread_cone.y + spread_cone.z) / 3.f);

	float random[2];
	const auto seed = md5::md5_pseudo_random(ucmd->command_number) & 0xFF;
	interfaces::random->set_seed(seed);

	random[0] = interfaces::random->random_float(-0.5f, 0.5f)
		+ interfaces::random->random_float(-0.5f, 0.5f);

	random[1] = interfaces::random->random_float(-0.5f, 0.5f)
		+ interfaces::random->random_float(-0.5f, 0.5f);

	auto dir = (c_vector(1.0f, 0.0f, 0.0f) + (c_vector(0.0f, -1.0f, 0.0f) * spread * random[0]) + (c_vector(0.0f, 0.0f, 1.0f) * spread * random[1]));

	q_angle out = math::get_angle(q_angle(0.f, 0.f, 0.f), dir);
	out = math::fix_angles(out);
	
	ucmd->viewangles += out;
}
