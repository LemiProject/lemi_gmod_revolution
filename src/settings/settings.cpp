#include "settings.h"

#include <fstream>

#include <file_tools.h>

#include "lemi_utils.h"
#include "../utils/game_utils.h"


settings::json json_settings;

std::string get_file_content(std::string_view path)
{
	std::ifstream lua_file;
	lua_file.open(path.data());
	std::string file_content((std::istreambuf_iterator<char>(lua_file)), std::istreambuf_iterator<char>());
	return file_content;
}

void settings::parse_settings_from_file(const std::string& s)
{
	parse_settings_from_string(get_file_content(s));
}

template <typename map_t>
void merge_map(const map_t& from, map_t& to)
{
	for (auto& i : to)
		for (auto j : from)
			if (i.first == j.first)
				i.second = j.second;
}

std::string settings::parse_setting_in_string()
{
	json j;
	j["states"] = states;
	j["values"] = values;
	j["strings"] = strings;
	j["binds"] = binds;
	j["colors"] = colors::colors_map;
	j["flags"] = flags;

	return j.dump();
}

void settings::parse_settings_in_file(const std::string& path)
{
	std::ofstream s(path.data());
	s.clear();
	s << parse_setting_in_string();
}

std::string settings::config_directory()
{
	auto path = file_tools::get_hack_directory_path();
	path.append("configs");
	if (!file_tools::exist(path.string()))
		file_tools::create_directory(path.string());
	return path.string();
}

std::vector<std::string> settings::get_configs()
{
	auto path = config_directory();
	std::vector<std::string> out;
	for (auto& p : std::filesystem::directory_iterator(path))
		if (!p.is_directory())
			if (p.path().filename().string().find(".vpcfg") != std::string::npos)
				out.push_back(p.path().filename().string());
	return out;
}

void settings::init_config_system()
{
	
}

int settings::lua_api::lua_api_get_hack_var__Imp(c_lua_interface* lua)
{
	CHECK_TYPE(lua, 1, (int)e_lua_type::type_string, "expected string", 0);

	std::string var = lua->get_string(1);

	if (var._Starts_with("states::"))
	{
		replace(var, "states::", "");

		if (states.find(var) == states.end())
		{
			lua->arg_error(1, "Cannot find var");
			return 0;
		}

		lua->push_bool(states.at(var));
		return 1;
	}
	if (var._Starts_with("binds::"))
	{
		replace(var, "binds::", "");

		if (binds.find(var) == binds.end())
		{
			lua->arg_error(1, "Cannot find var");
			return 0;
		}

		lua->push_number(binds.at(var));
		return 1;
	}
	if (var._Starts_with("values::"))
	{
		replace(var, "values::", "");

		if (values.find(var) == values.end())
		{
			lua->arg_error(1, "Cannot find var");
			return 0;
		}

		lua->push_number(values.at(var));
		return 1;
	}
	
	lua->arg_error(1, "Cannot find var");
	return 0;
}

int settings::lua_api::lua_api_is_key_down__Imp(c_lua_interface* lua)
{
	CHECK_TYPE(lua, 1, (int)e_lua_type::type_number, "expected number", 0);
	lua->push_bool(GetAsyncKeyState(lua->get_number(1)));
	return 1;
}

void settings::lua_api::push_all(c_lua_interface* lua)
{
	lua::push_cfunction(lua, "LGetHackVar", lua_api_get_hack_var);
	lua::push_cfunction(lua, "LIsKeyDown", lua_api_is_key_down);
	
}

const char* settings::visuals::to_string(const e_esp_box_type e)
{
	switch (e)
	{
	case e_esp_box_type::flat: return "Flat";
	case e_esp_box_type::bounding: return "Bounding";
	case e_esp_box_type::corners: return "Corners";
	default: return "unk";
	}
}

const char* settings::aimbot::to_string(e_player_filter e)
{
	switch (e)
	{
	case e_player_filter::fly: return "Fly";
	case e_player_filter::noclip: return "Noclip";
	case e_player_filter::observer: return "Observer";
	case e_player_filter::admin: return "Admin";
	default: return "unk";
	}
}

std::string settings::aimbot::to_string(e_player_bones e)
{
	switch (e)
	{
	case e_player_bones::pelvis: return "Pelvis";
	case e_player_bones::body: return "Body";
	case e_player_bones::top_body: return "TopBody";
	case e_player_bones::neck: return "Neck";
	case e_player_bones::head: return "Head";
	default: return "";
	}
}

void settings::visuals::c_entity_list::push_back(const std::string& c)
{
	//std::unique_lock<std::mutex> l(mutex);
	if (!exist(c))
		classes.push_back(c);
}

void settings::visuals::c_entity_list::remove(int idx)
{
	//std::unique_lock<std::mutex> l(mutex);
	classes.erase(classes.begin() + idx);
}

bool settings::visuals::c_entity_list::exist(const std::string& c)
{
	//std::lock_guard<std::mutex> l(mutex);
	return std::find(classes.begin(), classes.end(), c) != classes.end();
}

int settings::visuals::c_entity_list::find(const std::string& c)
{
	//std::lock_guard<std::mutex> l(mutex);
	if (!classes.empty())
		for (auto i = 0; i < classes.size(); ++i)
			if (classes.at(i) == c)
				return i;
	
	return -1;
}

void settings::visuals::c_entity_list::exchange(std::vector<std::string>& c)
{
	//std::unique_lock<std::mutex> l(mutex);
	classes = c;
}

bool settings::visuals::c_entity_list::empty()
{
	//std::lock_guard<std::mutex> l(mutex);
	return classes.empty();
}

void settings::visuals::c_entity_list::clear()
{
	classes.clear();
}

size_t settings::visuals::c_entity_list::size()
{
	return classes.size();
}

std::vector<std::string> settings::visuals::c_entity_list::data()
{
	return classes;
}

void settings::parse_settings_from_string(const std::string& s)
{
	auto j = json::parse(s);
	if (!j.empty())
	{
		if (!j["states"].empty())
			merge_map(j["states"].get <std::map<std::string, bool>>(), states);
		if (!j["values"].empty())
			merge_map(j["values"].get <std::map<std::string, float>>(), values);
		if (!j["strings"].empty())
			merge_map(j["strings"].get <std::map<std::string, std::string>>(), strings);
		if (!j["binds"].empty())
			merge_map(j["binds"].get<decltype(binds)>(), binds);
		if (!j["colors"].empty())
			merge_map(j["colors"].get<std::map<std::string, std::array<float, 4>>>(), colors::colors_map);
		if (!j["flags"].empty())
			merge_map(j["flags"].get<decltype(flags)>(), flags);
	}
}
