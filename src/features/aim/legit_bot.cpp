#include "aim.h"

#include "../../game_sdk/entitys/c_base_player.h"

#include "../../utils/game_utils.h"

#include "../../settings/settings.h"

struct target_t
{
	c_base_player* ply;
	float fov;
};

bool get_target(target_t& target)
{
	if (!get_local_player())
		return false;

	target_t tmp{nullptr, FLT_MAX};
	
	for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* player = get_player_by_index(i);

		if (!player || !player->is_player() || !player->is_alive())
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
		}
	}
}

void aim::legit_bot()
{
	
}
