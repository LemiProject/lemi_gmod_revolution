#include "prediction_system.h"

#include <cstdint>

#include "../../game_sdk/entitys/c_base_player.h"
#include "../../utils/memory_utils.h"

using namespace interfaces;

void c_prediction_system::start(c_user_cmd* cmd, c_base_player* player)
{
	m_player = player;
	if (!m_p_prediction_random_seed_)
		m_p_prediction_random_seed_ = *(int**)(memory_utils::pattern_scanner("client.dll", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 5D C3 8B 40 30") + 2);
	*m_p_prediction_random_seed_ = md5::md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF;

	m_fl_old_curtime_ = global_vars->curtime;
	m_fl_old_frametime_ = global_vars->frametime;

	global_vars->curtime = m_player->get_tick_base() * global_vars->interval_per_tick;
	global_vars->frametime = global_vars->interval_per_tick;

	game_movement->start_track_prediction_errors(m_player);
	memset(&m_move_data_, 0, sizeof(m_move_data_));
	prediction->setup_move(m_player, cmd, move_helper, &m_move_data_);
	game_movement->process_movement(m_player, &m_move_data_);
	prediction->finish_move(m_player, cmd, &m_move_data_);
}

void c_prediction_system::end()
{
	game_movement->finish_track_prediction_errors(m_player);
	*m_p_prediction_random_seed_ = -1;
	m_current_cmd = nullptr;
	global_vars->curtime = m_fl_old_curtime_;
	global_vars->frametime = m_fl_old_frametime_;
}

c_prediction_system engine_prediction;