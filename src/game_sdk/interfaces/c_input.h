#pragma once
#include <comcat.h>
#include "../../math/math.h"
#include "../misc/c_user_cmd.h"


class c_base_combat_weapon;
class camera_third_data_t;
class c_handle;
struct kbutton_t
{
	// key nums holding it down
	int		down[2];
	// low bit is down state
	int		state;
};
class c_keyboard_key
{
public:
	// Name for key
	char				name[32];
	// Pointer to the underlying structure
	kbutton_t* pkey;
	// Next key in key list.
	c_keyboard_key* next;
};
enum
{
	game_axis_none = 0,
	game_axis_forward,
	game_axis_pitch,
	game_axis_side,
	game_axis_yaw,
	max_game_axes
};
enum
{
	cam_command_none = 0,
	cam_command_tothirdperson = 1,
	cam_command_tofirstperson = 2
};
enum
{
	mouse_accel_threshhold1 = 0,	// if mouse moves > this many mickey's double it
	mouse_accel_threshhold2,		// if mouse moves > this many mickey's double it a second time
	mouse_speed_factor,				// 0 = disabled, 1 = threshold 1 enabled, 2 = threshold 2 enabled

	num_mouse_params,
};

class con_var;
class c_verified_user_cmd
{
public:
	c_user_cmd	m_cmd;
	uint32_t		m_crc;
};

using bf_write = void*;
using bf_read = void*;

using button_code_t = uint32_t;

class c_input
{
public:
	virtual		void		init_all(void) = 0;
	virtual		void		shutdown_all(void) = 0;
	virtual		int			get_button_bits(int) = 0;
	virtual		void		create_move(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual		void		extra_mouse_sample(float frametime, bool active) = 0;
	virtual		bool		write_usercmd_delta_to_buffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual		void		encode_user_cmd_to_buffer(bf_write& buf, int slot) = 0;
	virtual		void		decode_user_cmd_from_buffer(bf_read& buf, int slot) = 0;
	virtual		c_user_cmd* get_user_cmd(int sequence_number) = 0;
	virtual		void		make_weapon_selection(c_base_combat_weapon* weapon) = 0;
	virtual		float		key_state(kbutton_t* key) = 0;
	virtual		int			key_event(int down, button_code_t keynum, const char* pszCurrentBinding) = 0;
	virtual		kbutton_t* find_key(const char* name) = 0;
	virtual		void		controller_commands(void) = 0;
	virtual		void		joystick_advanced(void) = 0;
	virtual		void		joystick_set_sample_time(float frametime) = 0;
	virtual		void		in_set_sample_time(float frametime) = 0;
	virtual		void		accumulate_mouse(void) = 0;
	virtual		void		activate_mouse(void) = 0;
	virtual		void		deactivate_mouse(void) = 0;
	virtual		void		clear_states(void) = 0;
	virtual		float		get_look_spring(void) = 0;

	virtual		void		get_fullscreen_mouse_pos(int* mx, int* my, int* unclampedx = NULL, int* unclampedy = NULL) = 0;
	virtual		void		set_fullscreen_mouse_pos(int mx, int my) = 0;
	virtual		void		reset_mouse(void) = 0;

	//	virtual		bool		IsNoClipping( void );
	virtual		float		get_last_forward_move(void) = 0;
	virtual		float		joystick_get_forward(void) = 0;
	virtual		float		joystick_get_side(void) = 0;
	virtual		float		joystick_get_pitch(void) = 0;
	virtual		float		joystick_get_yaw(void) = 0;

	virtual		void		cam_think(void) = 0;
	virtual		int			cam_is_third_person(void) = 0;
	virtual		void		cam_to_third_person(void) = 0;
	virtual		void		cam_to_first_person(void) = 0;
	virtual		void		cam_start_mouse_move(void) = 0;
	virtual		void		cam_end_mouse_move(void) = 0;
	virtual		void		cam_start_distance(void) = 0;
	virtual		void		cam_end_distance(void) = 0;
	virtual		int			cam_intercepting_mouse(void) = 0;

	// orthographic camera info
	virtual		void		cam_to_orthographic() = 0;
	virtual		bool		cam_is_orthographic() = 0;
	virtual		void		cam_orthographic_size(float& w, float& h) const = 0;
public:
	BYTE pad__01[136]; // 0x8B & m_fCameraMovingWithMouse = true thirdperson working
	// Is the 3rd person camera using the mouse?
	bool		camera_intercepting_mouse;
	// Are we in 3rd person view?
	bool		camera_in_third_person;
	// Should we move view along with mouse?
	bool		camera_moving_with_mouse;
	// Is the camera in distance moving mode?
	bool		camera_distance_move; // 37
	// Old and current mouse position readings.
	int			camera_old_x;
	int			camera_old_y;
	int			camera_x;
	int			camera_y;
	// orthographic camera settings
	bool		camera_is_orthographic; // 1
	q_angle		previous_view_angles; // 12
	float		last_forward_move; // 4
	float previous_joystick_forward;
	float previous_joystick_side;
	float previous_joystick_pitch;
	float previous_joystick_yaw;
	c_user_cmd* commands;
	c_verified_user_cmd* verified_commands;
	camera_third_data_t* camera_third_data;
};