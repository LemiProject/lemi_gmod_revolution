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

		if (!glua->is_type(-1, (int)e_lua_type::type_entity))
		{
			glua->pop(1);
			return get_lua_script_name();
		}
		
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

		if (!glua->is_type(-1, (int)e_lua_type::type_entity))
		{
			glua->pop(1);
			return get_lua_script_name();
		}
		
		glua->get_field(-1, "GetClass");
		if (!glua->is_type(-1, (int)e_lua_type::type_string))
		{
			glua->pop(2);
			return get_lua_script_name();
		}

		std::string out = glua->get_string(-1);
		glua->pop(2);
		return out;
	}

	bool is_visible_by(c_base_entity* from)
	{
		ray_t ray;
		trace_t tr;
		c_trace_filter filter;
		filter.pSkip = from;

		c_vector eye_pos = from->get_eye_pos();
		c_vector end_pos = this->get_eye_pos();

		ray.init(eye_pos, end_pos);

		interfaces::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);


		if (tr.m_pEnt == this || tr.fraction >= 0.98f)
			return true;
		return false;
	}

	uint32_t get_bone_by_name(const std::string& name) const
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!lua)
			return 0;
		
		lua->push_special((int)e_special::glob); //1
		lua->get_field(-1, "Entity");
		lua->push_number(get_index()); //2
		lua->call(1, 1); // 2 - 1 = 1 + 1 = 2
		
		lua->get_field(-1, "LookupBone");
		lua->push(-2); //3
		lua->push_string(name.c_str()); //4
		lua->call(2, 1); // 4 - 2 = 2 + 1 = 3
		uint32_t id = lua->get_number(-1);
		lua->pop(3);
		return id;
	}

	c_vector get_bone(int bone)
	{
		matrix3x4_t bone_matrix[128];
		if (!setup_bones(bone_matrix, 128, 0x00000100, interfaces::engine->get_last_time_stamp()))
			return {};
		auto hitbox = bone_matrix[bone];
		return { hitbox[0][3], hitbox[1][3], hitbox[2][3] };
	}
};

__forceinline c_base_entity* get_entity_by_index(const int i)
{
	return static_cast<c_base_entity*>(interfaces::entity_list->get_entity_by_index(i));
}