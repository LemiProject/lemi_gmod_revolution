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
	NETVAR("DT_PlayerResource", "m_iPing", get_ping, int);
	//NETVAR("DT_BasePlayer", "m_viewPunchAngle", get_view_punch_angles, q_angle);
	
	int get_move_type()
	{
		auto* const glua = interfaces::lua_shared->get_interface((int)e_type::client);
		if (!glua)
			return {};
		c_lua_auto_pop p(glua);

		push_entity();
		
		glua->get_field(-1, "GetMoveType");
		glua->push(-2);
		glua->call(1, 1);

		return static_cast<int>(glua->get_number(-1));;
	}

	bool is_admin()
	{
		auto str = get_user_group();
		return str.find("admin") != std::string::npos || str.find("owner") != std::string::npos
		|| str.find("king") != std::string::npos || str.find("moder") != std::string::npos || str.find("root") != std::string::npos;
	}

	std::string get_team_name()
	{
		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
		if (!glua)
			return {};
		c_lua_auto_pop p(glua);

		glua->push_special((int)e_special::glob);
		glua->get_field(-1, "team");
		glua->get_field(-1, "GetName");
		glua->push_number(this->get_team_num());
		glua->call(1, 1);

		return glua->get_string();
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
		player_info_s info;
		interfaces::engine->get_player_info(get_index(), &info);
		return info.guid;
	}
	
	 q_angle get_view_punch_angles()
	 {
	 	auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
	 	lua->push_special((int)e_special::glob); //1
		push_entity();

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

	bool is_speaking()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		c_lua_auto_pop p(lua);

		push_entity();

		lua->get_field(-1, "IsSpeaking");
		lua->push(-2);
		lua->call(1, 1);

		return lua->get_bool();
	}
	
	q_angle get_view_offset_unduck()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		lua->push_special((int)e_special::glob); //1
		push_entity();

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
	
	//float get_ping()
	//{
	//	/*auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
	//	c_lua_auto_pop p(lua);
	//	push_entity();
	//	lua->get_field(-1, "Ping");
	//	lua->push(-2);
	//	lua->call(1, 1);
	//	return lua->get_number();*/
	//	auto ping = *(int*)((uintptr_t)this + netvar_manager::get_net_var(fnv::hash(), fnv::hash()) + (get_index() * 4));

	//	return ping;
	//}
	
	c_color get_team_color()
	{
		auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
		c_color color;

		if (!glua)
			return c_color();
		c_lua_auto_pop p(glua);

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

		color.init(r, g, b);
		return color;
	}
};


/// <summary>
/// Wrapper for local player
/// </summary>
class c_local_player : public c_base_player
{
public:
	//TODO: FIX THIS
	/*bool is_local_player_speaking()
	{
		/*using getm_fn = c_voice_status * (__cdecl*)();
		static getm_fn get_voice_status;
		if (!get_voice_status)
			get_voice_status = (getm_fn)(memory_utils::pattern_scanner("client.dll", "E8 ? ? ? ? 8B C8 E8 ? ? ? ? EB 16") + 1);

		using islps_fn = bool(__thiscall*)(c_voice_status*);
		static islps_fn is_lp_sp;
		if (!is_lp_sp)
			is_lp_sp = (islps_fn)memory_utils::pattern_scanner("client.dll", "8A 41 55");
		
		return is_lp_sp(get_voice_status());#1#
	}*/

	/// <summary>
	/// https://wiki.facepunch.com/gmod/Global.PrintMessage without lua
	/// </summary>
	/// <param name="str"></param>
	/// <param name="type">https://wiki.facepunch.com/gmod/Enums/HUD</param>
	void client_print(const std::string& str, int type) //55 8B EC 56 8B 75 10 85 F6 74 57
	{
		using fn = void(__cdecl*)(c_base_player*, int, const char*);
		static fn f;
		if (!f)
			f = reinterpret_cast<fn>(memory_utils::pattern_scanner("client.dll", "55 8B EC 56 8B 75 10 85 F6 74 57"));
		f(this, type, str.c_str());
	}

	c_vector get_view_offset(bool duck = true) //IDA moment...
	{
		c_vector tmp;
		auto out = interfaces::game_movement->get_player_view_offset(tmp, duck);
		return out;
	}
};

__forceinline c_local_player* get_local_player()
{
	return static_cast<c_local_player*>(interfaces::entity_list->get_entity_by_index(interfaces::engine->get_local_player()));
}


__forceinline c_base_player* get_player_by_index(const int i)
{
	return static_cast<c_base_player*>(interfaces::entity_list->get_entity_by_index(i));
}

//__forceinline void spawn_prop(c_base_player* lp, const std::string& str, )