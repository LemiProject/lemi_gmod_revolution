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

class c_base_player : public c_base_entity //278
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
	NETVAR("DT_BasePlayer", "m_nTickBase", get_tick_base, int);
	NETVAR("DT_BasePlayer", "m_nNextThinkTick", get_next_think_tick, float);
	NETVAR("DT_BasePlayer", "m_flFriction", get_friction, float);
	NETVAR("DT_BasePlayer", "flDeathTime", get_death_time, float);
	NETVAR("DT_BasePlayer", "deadflag", get_dead_flag, int);
	NETVAR("DT_BasePlayer", "m_iDefaultFOV", get_default_fov, int);
	NETVAR("DT_BasePlayer", "m_hVehicle", get_vehicle_handle, uintptr_t);
	NETVAR("DT_BasePlayer", "m_iObserverMode", get_observer_mode, int);
	NETVAR("DT_BasePlayer", "m_hObserverTarget", get_observer_target_handle, uintptr_t);

	int get_move_type()
	{
		auto* const glua = interfaces::lua_shared->get_interface((int)e_type::client);
		if (!glua)
			return {};
		push_entity(); //1
		
		glua->get_field(-1, "GetMoveType");
		glua->push(-2); //2
		glua->call(1, 1); // 2 - 1 = 1 + 1 = 2

		int type = static_cast<int>(glua->get_number(-1));
		glua->pop(2);

		return type;
	}

	//void set_local_view_angles(q_angle ang)
	//{
	//	using fn = void(__thiscall*)(void*, q_angle);
	//	return (*(fn**)this)[350](this, ang);
	//}
	
	bool is_admin()
	{
		auto str = get_user_group();
		return str.find("admin") != std::string::npos || str.find("owner") != std::string::npos
		|| str.find("king") != std::string::npos || str.find("moder") != std::string::npos;
	}

	std::string get_team_name()
	{
		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);

		if (!glua)
			return {};

		glua->push_special((int)e_special::glob); //1
		glua->get_field(-1, "team"); //2
		glua->get_field(-1, "GetName"); //2
		glua->push_number(this->get_team_num()); //3
		glua->call(1, 1); // 3
		std::string out = glua->get_string();
		glua->pop(3);
		return out;
	}

	std::string get_user_group()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!lua)
			return {};
		c_lua_auto_pop p(lua);
		
		push_entity();
		lua->get_field(-1, "GetUserGroup");
		lua->push(-2);
		lua->call(1, 1);
		return lua->get_string();
	}
	
	std::string get_name() const
	{
		//auto* const glua = interfaces::lua_shared->get_interface((int)e_type::client);

		//if (!glua)
		//	return {};

		//glua->push_special(static_cast<int>(e_special::glob)); //1
		//glua->get_field(-1, "Entity"); //1
		//glua->push_number(this->get_index()); //2
		//glua->call(1, 1); // 2

		//glua->get_field(-1, "Name"); //2
		//glua->push(-2); //3
		//glua->call(1, 1); // 3

		//std::string name = glua->get_string(-1);
		//glua->pop(3);

		player_info_s info;
		interfaces::engine->get_player_info(get_index(), &info);
		return info.name;
	}

	float get_armor()
	{
		auto* const lua = interfaces::lua_shared->get_interface((int)e_type::client);
		if (!lua)
			return 0.f;
		c_lua_auto_pop p(lua);
		push_entity();
		lua->get_field(-1, "Armor");
		lua->push(-2);
		lua->call(1, 1);
		
		return (float)lua->get_number();
	}

	std::string get_steam_id() const
	{
		//auto* const glua = interfaces::lua_shared->get_interface((int)e_type::client);

		//if (!glua)
		//	return {};

		//glua->push_special(static_cast<int>(e_special::glob)); //1
		//glua->get_field(-1, "Entity"); //1
		//glua->push_number(this->get_index()); //2
		//glua->call(1, 1); // 2

		//glua->get_field(-1, "SteamID"); //2
		//glua->push(-2); //3
		//glua->call(1, 1); // 3
		//if (!glua->is_type(-1, (int)e_lua_type::type_string))
		//{
		//	glua->pop(3);
		//	return {};
		//}
		//
		//std::string sid = glua->get_string(-1);
		//glua->pop(3);

		//return sid;

		player_info_s info;
		interfaces::engine->get_player_info(get_index(), &info);
		return info.guid;
	}
	
	q_angle get_view_punch_angles()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		lua->push_special((int)e_special::glob); //1
		lua->get_field(-1, "Entity");
		lua->push_number(get_index()); //2
		lua->call(1, 1); //2 - 1 = 1 + 1 = 2  AJFHA FUCKING LUA AAKSJDKSADJLKASJDLKAJSDLKJASD

		lua->get_field(-1, "GetViewPunchAngles");
		lua->push(-2);
		lua->call(1, 1);

		lua->push_string("x");
		lua->get_table(-2);
		float x = lua->get_number(-1);
		lua->pop();

		lua->push_string("y");
		lua->get_table(-2);
		float y = lua->get_number(-1);
		lua->pop();

		lua->push_string("z");
		lua->get_table(-2);
		float z = lua->get_number(-1);
		lua->pop();

		lua->pop(3);

		return {x, y, z};
	}

	q_angle get_view_offset_unduck()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		lua->push_special((int)e_special::glob); //1
		lua->get_field(-1, "Entity");
		lua->push_number(get_index()); //2
		lua->call(1, 1); //2 - 1 = 1 + 1 = 2  AJFHA FUCKING LUA AAKSJDKSADJLKASJDLKAJSDLKJASD

		lua->get_field(-1, "GetViewOffset");
		lua->push(-2);
		lua->call(1, 1);

		lua->push_string("x");
		lua->get_table(-2);
		float x = lua->get_number(-1);
		lua->pop();

		lua->push_string("y");
		lua->get_table(-2);
		float y = lua->get_number(-1);
		lua->pop();

		lua->push_string("z");
		lua->get_table(-2);
		float z = lua->get_number(-1);
		lua->pop();

		lua->pop(3);

		return { x, y, z };
	}
	
	float get_ping()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		c_lua_auto_pop p(lua);
		push_entity();
		lua->get_field(-1, "Ping");
		lua->push(-2);
		lua->call(1, 1);
		return lua->get_number();
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

//__forceinline void spawn_prop(c_base_player* lp, const std::string& str, )