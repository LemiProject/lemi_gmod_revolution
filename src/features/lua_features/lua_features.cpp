#include "lua_features.h"


#include <filesystem>
#include <iostream>
#include <vector>
#include <mutex>

#include "file_tools.h"
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
		std::unique_lock l(run_mutex);
		codes_to_run.swap(codes_to_run_safe);
	}
}

void lua_features::run_all_code()
{
	std::unique_lock l(run_mutex);
	is_hack_call = true;
	
	auto lua = interfaces::lua_shared->get_interface((int)e_type::client);
	if (lua)
	{
		for (auto i : codes_to_run_safe)
		{
			try 
			{
				lua::push_hack_globals(lua), lua->run_string(last_name.c_str(), "", i.c_str(), true, true);
			}
			catch (...)
			{
			}
		}
	}

	codes_to_run_safe.clear();
	is_hack_call = false;
}

std::vector<std::string> get_autorun_luas()
{
	auto get_lua_dir = [&]()
	{
		auto dir_path = file_tools::get_hack_directory_path();
		dir_path.append("lua");
		if (!file_tools::exist(dir_path.string()))
			file_tools::create_directory(dir_path.string());
		dir_path.append("autorun");
		if (!file_tools::exist(dir_path.string()))
			file_tools::create_directory(dir_path.string());
		return dir_path.string();
	};

	auto get_luas = [&]()
	{
		auto path = get_lua_dir();
		std::vector<std::string> out;
		for (auto& p : std::filesystem::directory_iterator(path))
			if (!p.is_directory())
				if (p.path().filename().string().find(".lua") != std::string::npos)
					out.push_back(p.path().filename().string());
		return out;
	};

	return get_luas();
}

void lua_features::run_auto_run(c_lua_interface* intr)
{
	//is_hack_call = true;

	auto get_lua_dir = [&]()
	{
		auto dir_path = file_tools::get_hack_directory_path();
		dir_path.append("lua");
		if (!file_tools::exist(dir_path.string()))
			file_tools::create_directory(dir_path.string());
		dir_path.append("autorun");
		if (!file_tools::exist(dir_path.string()))
			file_tools::create_directory(dir_path.string());
		return dir_path.string();
	};


	auto lua = interfaces::lua_shared->get_interface((int)e_type::client);
	if (lua) {
		for (const auto& i : get_autorun_luas()) {
			try {
				if (!std::filesystem::exists(get_lua_dir() + "\\" + i))
					continue;

				std::string content;
				file_tools::read_file(content, get_lua_dir() + "\\" + i);

				add_code_to_run(content);

				std::cout << i << "\tloaded as autorun" << std::endl;
			}
			catch (...) {
			}
		}
	}
	//is_hack_call = false;
}

std::string lua_code::lemi_code = u8R"(
local gaysex=debug.getinfo
debug.getinfo=function(gay)
  local info = gaysex(gay)
  if info == nil then
                data123.lastlinedefined = 40
            data123.linedefined = 38
            data123.nups = 0
            data123.func = "function: 0x7285B6FA"
            data123.short_src = "lua/includes/extensions/player_auth.lua"
            data123.source = "@lua/includes/extensions/player_auth.lua"
            data123.what = "LUA"
return data123
  end
  return info 
end

)";
