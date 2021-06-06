#pragma once
#include "../../../game_sdk/entitys/c_base_weapon.h"


inline void calc_spread_weapon_base(c_base_combat_weapon* weapon, c_user_cmd* ucmd)
{
	//auto spread = weapon->get_bullet_spread();
	//
	//auto cone = (spread.x + spread.y + spread.z) / 3.f;

	//if (weapon->is_use_lua())
	//	cone = weapon->get_primary_value("Spread");
	//
	//auto seed = md5::md5_pseudo_random(ucmd->command_number) & 0xFF;

	//interfaces::random->set_seed(seed);
	//
	//auto x = interfaces::random->random_float(-1.f, 1.f)
	//	+ interfaces::random->random_float(-1.f, 1.f);
	//auto y = interfaces::random->random_float(-1.f, 1.f)
	//	+ interfaces::random->random_float(-1.f, 1.f);

	//c_vector dir{ 1.f, cone * y, -(cone * x)};
	//q_angle ang;
	//math::vector_to_angel(dir, ang);
	
	//auto spread = weapon->get_bullet_spread();
	//auto seed = md5::md5_pseudo_random(ucmd->command_number) & 0xFF;
	//interfaces::random->set_seed(seed);


	//c_vector random;
	//random.x = interfaces::random->random_float(-0.5f, 0.5f) + interfaces::random->random_float(-0.5f, 0.5f);
	//random.y = interfaces::random->random_float(-0.5f, 0.5f) + interfaces::random->random_float(-0.5f, 0.5f);

	//spread.x = random.x * spread.x;
	//spread.y = random.y * spread.y;
	//spread.z = 0.f;
	//
	//c_vector forward, right, up;
	//{
	//	float sr, sp, sy, cr, cp, cy;
	//	float radx = math::deg2rad(ucmd->viewangles.x);
	//	float rady = math::deg2rad(ucmd->viewangles.y);
	//	float radz = math::deg2rad(ucmd->viewangles.z);
	//	sp = sin(radx); cp = cos(radx);
	//	sy = sin(rady); cy = cos(rady);
	//	sr = sin(radz); cr = cos(radz);

	//	forward.x = cp * cy;
	//	forward.y = cp * sy;
	//	forward.z = -sp;

	//	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	//	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	//	right.z = -1 * sr * cp;

	//	up.x = (cr * sp * cy + -sr * -sy);
	//	up.y = (cr * sp * sy + -sr * cy);
	//	up.z = cr * cp;
	//}

	//auto dir = forward + (right * spread.x) + (up * spread.y);
	//q_angle ang;
	//math::vector_to_angel(dir, ang);
	//ang.normalize();


	//q_angle out = ucmd->viewangles - ang;
	//ucmd->viewangles -= out;
}
