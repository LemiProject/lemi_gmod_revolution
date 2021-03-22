#include "settings.h"

#include <fstream>

settings::json json_settings;
std::vector<settings::c_config_item> config_items;



std::string get_file_content(std::string_view path)
{
	std::ifstream lua_file;
	lua_file.open(path.data());
	std::string file_content((std::istreambuf_iterator<char>(lua_file)), std::istreambuf_iterator<char>());
	return file_content;
}

void settings::parse_settings_from_file(std::string_view path)
{
	parse_settings_from_string(get_file_content(path));
}

std::string settings::parse_setting_in_string()
{
	return { };
	//return current_settings.dump();
}

void settings::parse_settings_in_file(std::string_view path)
{
	//std::ofstream file_s;
	//file_s.open(path.data(), std::ios::app);
	//file_s.clear();
	//file_s << current_settings.dump();
}

void settings::init_config_system()
{
	
}

void settings::update_settings()
{
	/*current_settings["visuals"]["esp"] = visuals::esp;
	current_settings["visuals"]["esp_box"] = visuals::esp_box;
	current_settings["visuals"]["esp_box_type"] = visuals::esp_box_type;
	current_settings["visuals"]["esp_health"] = visuals::esp_health;
	current_settings["visuals"]["esp_name"] = visuals::esp_name;*/
	
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

void settings::parse_settings_from_string(std::string_view string)
{
	auto j = json::parse(string.data());
	
	visuals::esp = j["visuals"]["esp"];
	visuals::esp = j["visuals"]["esp_box"];
	visuals::esp = j["visuals"]["esp_box_type"];
	visuals::esp = j["visuals"]["esp_health"];
	visuals::esp = j["visuals"]["esp_name"];
	
	//current_settings = j;
}
