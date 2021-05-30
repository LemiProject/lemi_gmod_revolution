#pragma once

#include <string>

namespace lua_features
{
	inline std::string last_name;
	inline std::string last_path;
	
	using run_str_fn = bool(__thiscall*)(void*, const char*, const char*, const char*, bool, bool, bool, bool);
	
	void add_code_to_run(const std::string& str);

	void run_all_code();
}

namespace lua_code
{

	extern std::string lemi_code;
}