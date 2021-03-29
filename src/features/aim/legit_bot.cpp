#include <iostream>

#include "aim.h"

#include "../../game_sdk/entitys/c_base_player.h"

#include "../../utils/game_utils.h"

#include "../../settings/settings.h"

struct target_t
{
	c_base_player* ply;
	float fov;
	c_vector angle;
};

bool get_target(target_t& target)
{
	auto invalidate_target = [](const target_t& t)
	{
		return (t.ply != nullptr && t.angle.x != 0 && t.angle.y != 0); //fov maybe FLT_MAX and 0
	};
	
	if (!get_local_player())
		return false;

	target_t tmp{nullptr, FLT_MAX, {}};
	
	for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* player = get_player_by_index(i);

		if (!player || !player->is_player() || !player->is_alive() || player == get_local_player())
			continue;

		c_vector engine_angels;
		interfaces::engine->get_view_angles(engine_angels);
		const auto fov = game_utils::get_fov(engine_angels,
		                                     game_utils::calc_angle(get_local_player()->get_eye_pos(),
		                                                            player->get_eye_pos()));
		
		if (fov < tmp.fov && fov <= settings::aim::legit_bot_fov)
		{
			tmp.fov = fov;
			tmp.ply = player;
			tmp.angle = game_utils::calc_angle(get_local_player()->get_eye_pos(), player->get_eye_pos());
		}
	}

	if (!invalidate_target(tmp))
		return false;
		
	target = tmp;
	return true;
}

void aim::legit_bot(c_user_cmd* cmd)
{
	//if (/*!settings::aim::legit_bot_enabled ||*/ !interfaces::engine->is_in_game())
	//	return;

	//auto* const lp = get_local_player();

	//if (!cmd|| !lp || !lp->is_alive() || !lp->is_alive())
	//	return;

	//target_t target;

	////if (!(cmd->buttons & IN_ATTACK))
	////	return;
	//
	//if (!get_target(target))
	//	return;
	//
	//cmd->viewangles = target.angle;
}
