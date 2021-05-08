#pragma once
#include "../../math/math.h"

class c_move_data
{
public:
	char pad_0000[4]; //0x0000
	uint32_t m_n_player_handle; //0x0004
	int32_t m_n_impulse_command; //0x0008
	q_angle m_vec_view_angles; //0x000C
	q_angle m_vec_abs_view_angles; //0x0018
	int32_t m_n_buttons; //0x0024
	int32_t m_n_old_buttons; //0x0028
	float m_fl_forward_move; //0x002C
	float m_fl_side_move; //0x0030
	float m_fl_up_move; //0x0034
	float m_fl_max_speed; //0x0038
	float m_fl_client_max_speed; //0x003C
	c_vector m_vec_velocity; //0x0040
	q_angle m_vec_angles; //0x004C
	q_angle m_vec_old_angles; //0x0058
	float m_out_step_height; //0x0064
	c_vector m_out_wish_vel; //0x0068
	c_vector m_out_jump_vel; //0x0074
	c_vector m_vec_constraint_center; //0x0080
	float m_fl_constraint_radius; //0x008C
	float m_fl_constraint_width; //0x0090
	float m_fl_constraint_speed_factor; //0x0094
	c_vector m_vec_abs_origin; //0x0098
};