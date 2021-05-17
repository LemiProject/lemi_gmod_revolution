#include "settings.h"

#include <fstream>

#include <file_tools.h>

settings::json json_settings;
std::vector<settings::c_config_item> config_items;

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

std::string settings::parse_setting_in_string()
{
	json j;
	j["states"] = states;
	j["values"] = values;
	j["strings"] = strings;
	j["binds"] = binds;
	
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
		states = j["states"].get <std::map<std::string, bool>>();
		values = j["values"].get <std::map<std::string, float>>();
		strings = j["strings"].get <std::map<std::string, std::string>>();
	}
}
