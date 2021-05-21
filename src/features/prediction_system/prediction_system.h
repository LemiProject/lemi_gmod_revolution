#pragma once
#include "../../game_sdk/entitys/c_base_player.h"
#include "../../game_sdk/misc/c_user_cmd.h"

class c_prediction_system {
public:
	void start(c_user_cmd* cmd, c_base_player* player);
	void end();

	int get_prediction_random_seed() const
	{
		return *m_p_prediction_random_seed_;
	}
private:
	float m_fl_old_curtime_;
	float m_fl_old_frametime_;

	c_base_player* m_player;
	c_move_data m_move_data_ = { };
	c_user_cmd* m_current_cmd;
	int* m_p_prediction_random_seed_;
}; extern c_prediction_system engine_prediction;