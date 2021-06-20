#pragma once

#include <string>
#include "../../game_sdk/interfaces/lua_shared.h"


namespace lua_features
{
	inline std::string last_name;
	inline std::string last_path;
	inline bool is_hack_call;
	
	using run_str_fn = bool(__thiscall*)(void*, const char*, const char*, const char*, bool, bool, bool, bool);
	
	void add_code_to_run(const std::string& str);

	void run_all_code();

	void run_auto_run(c_lua_interface* intr);
}

namespace lua_code
{

	extern std::string lemi_code;
}