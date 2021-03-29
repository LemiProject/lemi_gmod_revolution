#pragma once
#include "c_base_entity.h"

enum class e_move_type
{
	none = 0,
	isometric,
	walk,
	step,
	fly,
	flygravity,
	vphysics,
	push,
	noclip,
	ladder,
	observer,
	custom,
	last = custom,
	max_bits = 4,
	movetype
};

class c_base_player : public c_base_entity
{
public:
	NETVAR("DT_BasePlayer", "m_fFlags", get_flags, int);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", get_view_offset, c_vector);
	NETVAR("DT_BasePlayer", "m_hActiveWeapon", active_weapon_handle, uintptr_t);
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", get_velocity, c_vector);
	NETVAR("DT_BasePlayer", "hMyWeapons", get_weapons, uintptr_t);
	NETVAR("DT_BasePlayer", "m_bloodColor", get_blood_color, c_vector);
	NETVAR("DT_BasePlayer", "m_Local", get_local, uintptr_t);
	NETVAR("DT_BasePlayer", "m_iAmmo", get_ammo, int);
	NETVAR("DT_BasePlayer", "m_fOnTarget", get_on_target, int);
	NETVAR("DT_BasePlayer", "m_nTickBase", get_tick_base, float);
	NETVAR("DT_BasePlayer", "m_nNextThinkTick", get_next_think_tick, float);
	NETVAR("DT_BasePlayer", "m_flFriction", get_friction, float);
	NETVAR("DT_BasePlayer", "flDeathTime", get_death_time, float);
	NETVAR("DT_BasePlayer", "deadflag", get_dead_flag, int);
	NETVAR("DT_BasePlayer", "m_iDefaultFOV", get_default_fov, int);
	NETVAR("DT_BasePlayer", "m_hVehicle", get_vehicle_handle, uintptr_t);
	NETVAR("DT_BasePlayer", "m_iObserverMode", get_observer_mode, int);
	NETVAR("DT_BasePlayer", "m_hObserverTarget", get_observer_target_handle, uintptr_t);

	[[nodiscard]] std::string get_name() const
	{
		auto* const glua = interfaces::lua_shared->get_interface((int)e_type::client);

		if (!glua)
			return {};

		glua->push_special(static_cast<int>(e_special::glob));
		glua->get_field(-1, "Entity");
		glua->push_number(this->get_index());
		glua->call(1, 1);

		glua->get_field(-1, "Name");
		glua->push(-2);
		glua->call(1, 1);

		std::string name = glua->get_string(-1);
		glua->pop(3);

		return name;
	}

	c_color get_team_color()
	{
		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
		c_color color;

		if (!glua)
			return c_color();

		glua->push_special((int)e_special::glob);

		glua->push_special((int)e_special::glob);
		glua->get_field(-1, "team");
		glua->get_field(-1, "GetColor");
		glua->push_number(this->get_team_num());
		glua->call(1, 1);

		glua->push_string("r");
		glua->get_table(-2);
		int r = glua->get_number(-1);
		glua->pop();

		glua->push_string("g");
		glua->get_table(-2);
		int g = glua->get_number(-1);
		glua->pop();

		glua->push_string("b");
		glua->get_table(-2);
		int b = glua->get_number(-1);
		glua->pop();

		glua->pop(4);

		color.init(r, g, b);
		return color;
	}
};

__forceinline c_base_player* get_local_player()
{
	return static_cast<c_base_player*>(interfaces::entity_list->get_entity_by_index(interfaces::engine->get_local_player()));
}


__forceinline c_base_player* get_player_by_index(const int i)
{
	return static_cast<c_base_player*>(interfaces::entity_list->get_entity_by_index(i));
}