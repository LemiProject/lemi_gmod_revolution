#include "lua_features.h"


#include <iostream>
#include <vector>
#include <mutex>

#include "../../interfaces.h"
#include "../../utils/game_utils.h"

std::mutex run_mutex;

std::vector<std::string> codes_to_run;
std::vector<std::string> codes_to_run_safe;

void lua_features::add_code_to_run(const std::string& str)
{
	codes_to_run.clear();
	codes_to_run.push_back(str);
	
	{
		std::unique_lock<std::mutex> l(run_mutex);
		codes_to_run.swap(codes_to_run_safe);
	}
}

void lua_features::run_all_code()
{
	std::unique_lock<std::mutex> l(run_mutex);
	auto lua = interfaces::lua_shared->get_interface((int)e_type::client);
	if (lua)
	{
		for (auto i : codes_to_run_safe)
		{
			try 
			{
				lua::push_hack_globals(lua), lua->run_string("shared.lua", "lua/autorun/client/dinit.lua", i.c_str(), true, true);
			}
			catch (...)
			{
			}
		}
	}

	codes_to_run_safe.clear();
}
