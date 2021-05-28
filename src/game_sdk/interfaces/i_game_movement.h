#pragma once

#include "../misc/c_move_data.h"

struct trace_t;

class c_base_handle;
typedef c_base_handle entity_handle_t;

class c_entity;

class i_game_movement
{
public:
	virtual	~i_game_movement(void) = 0;
	virtual void	process_movement(void* pPlayer, c_move_data* pMove) = 0;
	virtual void	start_track_prediction_errors(void* pPlayer) = 0;
	virtual void	finish_track_prediction_errors(void* pPlayer) = 0;
	virtual void	idk() = 0;
	virtual c_vector get_player_mins(bool ducked) = 0;
	virtual c_vector get_player_maxs(bool ducked) = 0;
	virtual c_vector const& get_player_view_offset(bool ducked)= 0;
	virtual void	trace_player_b_box(const c_vector& start, const c_vector& end, unsigned int fMask, int collisionGroup, trace_t& pm) = 0;
	virtual void	try_touch_ground(const c_vector& start, const c_vector& end, const c_vector& mins, const c_vector& maxs, unsigned int fMask, int collisionGroup, trace_t& pm) = 0;
	virtual void	idk5() = 0;
	virtual void	player_move(void) = 0;
	virtual float	calc_roll(const q_angle& angles, const c_vector& velocity, float rollangle, float rollspeed) = 0;
	virtual	void	decay_punch_angle(void) = 0;
	virtual void	check_water_jump(void) = 0;
	virtual void	water_move(void) = 0;
	virtual void	air_accelerate(c_vector& wishdir, float wishspeed, float accel) = 0;
	virtual void	air_move(void) = 0;
	virtual float	get_air_speed_cap(void) = 0;
	virtual bool	can_accelerate() = 0;
	virtual void	accelerate(c_vector& wishdir, float wishspeed, float accel) = 0;
	virtual void	walk_move(void) = 0;
	virtual void	full_walk_move() = 0;
	virtual void	idk6() = 0;
	virtual void	idk7() = 0;
	virtual void	idk8() = 0;
	typedef enum
	{
		ground = 0,
		stuck,
		ladder
	} interval_type_t;
	virtual int	get_check_interval(interval_type_t type) = 0;
	// Returns true if he started a jump (ie: should he play the jump animation)?
	virtual bool	check_jump_button(void) = 0; // Overridden by each game.
	virtual void	full_toss_move(void) = 0;
	virtual void	full_ladder_move() = 0;
	// The basic solid body movement clip that slides along multiple planes
	virtual int		try_player_move(c_vector* pFirstDest = NULL, trace_t* pFirstTrace = NULL) = 0;
	virtual bool	ladder_move(void) = 0;
	virtual bool	on_ladder(trace_t& trace) = 0;
	virtual float	ladder_distance(void) const { return 2.0f; }
	virtual unsigned int ladder_mask(void) const { return (0x1|0x4000|0x10000|0x2|0x2000000|0x8); }
	virtual float	climb_speed(void) const { return 200; }
	virtual float	ladder_lateral_multiplier(void) const { return 1.0f; }
	virtual int		check_stuck(void) = 0;
	virtual bool	check_water(void) = 0;
	virtual void	categorize_position(void) = 0;
	virtual void	check_parameters(void) = 0;
	virtual	void	reduce_timers(void) = 0;
	virtual void	check_falling(void) = 0;
	virtual void	player_rough_landing_effects(float fvol) = 0;
	virtual void	duck(void) = 0;
	virtual void	handle_ducking_speed_crop() = 0;
	virtual void	finish_un_duck(void) = 0;
	virtual void	finish_duck(void) = 0;
	virtual bool	can_unduck() = 0;
	virtual c_base_handle test_player_position(const c_vector& pos, int collisionGroup, trace_t& pm) = 0;
	virtual void	set_ground_entity(trace_t* pm) = 0;
	virtual void	step_move(c_vector& vecDestination, trace_t& trace) = 0;
	virtual void	idk04() = 0;
	virtual bool	game_has_ladders() const = 0;

};