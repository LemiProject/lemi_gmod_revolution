#pragma once
#include "../../math/math.h"
#include "../misc/c_user_cmd.h"


class i_move_helper;
class c_move_data;

class i_prediction {
public:
	virtual	~i_prediction(void) = 0;
	virtual void	init(void) = 0;
	virtual void	shutdown(void) = 0;
	virtual void	update
	(
		int startframe,		// World update ( un-modded ) most recently received
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;
	virtual void	pre_entity_packet_received(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	post_entity_packet_received(void) = 0;
	virtual void	post_network_data_received(int commands_acknowledged) = 0;
	virtual void	on_received_uncompressed_packet(void) = 0;
	virtual void	get_view_origin(c_vector& org) = 0;
	virtual void	set_view_origin(c_vector& org) = 0;
	virtual void	get_view_angles(q_angle& ang) = 0;
	virtual void	set_view_angles(q_angle& ang) = 0;
	virtual void	get_local_view_angles(q_angle& ang) = 0;
	virtual void	set_local_view_angles(q_angle& ang) = 0;
	virtual bool	in_prediction(void) = 0;
	virtual bool	is_first_time_predicted(void) = 0;
	virtual int		get_incoming_packet_number(void) = 0;
	virtual void	run_command(c_base_entity* player, c_user_cmd* ucmd, i_move_helper* moveHelper) = 0;
	virtual void	setup_move(c_base_entity* player, c_user_cmd* ucmd, i_move_helper* pHelper, c_move_data* move) = 0;
	virtual void	finish_move(c_base_entity* player, c_user_cmd* ucmd, c_move_data* move) = 0;
	virtual void	set_ideal_pitch(c_base_entity* player, const c_vector& origin, const q_angle& angles, const c_vector& viewheight) = 0;
	virtual void	update
	(
		bool received_new_world_update,
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;
};
