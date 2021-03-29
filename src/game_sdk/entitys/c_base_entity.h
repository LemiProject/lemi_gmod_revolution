#pragma once
#include "color.h"
#include "../../utils/netvars.h"
#include "../../interfaces.h"

#include "i_client_entity.h"



class c_collidable
{
public:
	virtual void unknown_0() = 0;
	virtual c_vector& mins() = 0;
	virtual c_vector& maxs() = 0;
};

class c_base_entity : public i_client_entity
{
public:
	NETVAR("DT_BaseEntity", "m_vecOrigin", get_origin, c_vector);
	NETVAR("DT_BaseEntity", "m_vecAngles", get_angels, c_vector);
	NETVAR("DT_BaseEntity", "m_nModelIndex", get_model_index, int);
	NETVAR("DT_BaseEntity", "m_iTeamNum", get_team_num, int);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", get_view_offset, c_vector);
	NETVAR("DT_BaseEntity", "m_flAnimTime", get_anim_time, float);
	NETVAR("DT_BaseEntity", "m_flSimulationTime", get_simulation_time, float);
	NETVAR("DT_BaseEntity", "m_angRotation", get_rotation, c_vector);
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", get_owner_entity_handle, uintptr_t);
	NETVAR("DT_BaseEntity", "m_iHealth", get_health, int);
	NETVAR("DT_BaseEntity", "m_iMaxHealth", get_max_health, int);

	
	c_vector get_eye_pos() { return this->get_origin() + this->get_view_offset(); }
	
	c_collidable* get_collidable_ptr()
	{
		using original_fn = c_collidable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	bool is_alive()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[129](this);
	}

	bool is_player()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[130](this);
	}

	void push_entity() //https://imgur.com/bK6cNdu
	{
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)this)[172](this);
	}

	bool is_use_lua()
	{
		using fn = bool(__thiscall*)(void*);
		return (*(fn**)this)[170](this);
	}

	const char* get_lua_script_name() //B8 AA 2A 4D 00 C3	client.dll
	{
		using fn = const char* (__thiscall*)(void*);
		return (*(fn**)this)[183](this);
	}
	
	std::string get_print_name()
	{
		if (!is_use_lua())
			return {};

		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!glua)
			return {};
		push_entity();

		glua->get_field(-1, "PrintName");
		if (!glua->is_type(-1, (int)e_lua_type::type_string))
		{
			glua->pop(2);
			return this->get_lua_script_name();
		}

		std::string out = glua->get_string(-1);
		glua->pop(2);
		return out;
	}

	std::string get_class_name()
	{
		if (!is_use_lua())
			return {};

		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!glua)
			return {};
		push_entity();

		glua->get_field(-1, "GetClass");
		if (!glua->is_type(-1, (int)e_lua_type::type_string))
		{
			glua->pop(2);
			return this->get_lua_script_name();
		}

		std::string out = glua->get_string(-1);
		glua->pop(2);
		return out;
	}
};

__forceinline c_base_entity* get_entity_by_index(const int i)
{
	return static_cast<c_base_entity*>(interfaces::entity_list->get_entity_by_index(i));
}