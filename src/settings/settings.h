#pragma once
#include <any>
#include <string>
#include <json.hpp>
#include <vector>
#include <map>

#include "../game_sdk/entitys/c_base_entity.h"

namespace settings
{
	using json = nlohmann::json;

	class c_config_item
	{
	public:
		template <typename T>
		c_config_item(T& value, std::string_view name)
		{
			this->name = name;
			this->value = reinterpret_cast<void*>(value);
			this->type = typeid(T).name();
		}

		std::string name;
		std::string type;
		void* value;
	};
	
	namespace visuals
	{
		enum class e_esp_box_type
		{	
			flat,
			bounding,
			corners,
			iter_last //For creating foreach impl for (auto i = 0; i < iter_last; ++i) { //code }
		}; const char* to_string(e_esp_box_type e);

		inline auto esp = true;
		inline auto esp_box = true;
		inline auto esp_name = true;
		inline auto esp_health = true;

		inline auto esp_box_color_by_team = false;
		
		inline int esp_box_type = static_cast<int>(e_esp_box_type::bounding);

		inline std::vector<std::string> entitys_to_draw = {};
	}

	namespace aim
	{
		inline bool legit_bot_enabled = false;
		inline float legit_bot_fov = 30.f;

		
	}
	
	namespace colors
	{
		typedef std::array<float, 4> color_t;
		inline std::map<std::string, color_t> colors_map
		{
			{"esp_box_color", {1, 0, 0, 0.8f}},
			{"esp_health_color_hp", {0, 1, 0, 0.8f}},
			{"esp_health_color_void", {0, 0, 0, 0}}
		};
	}
	
	void parse_settings_from_string(std::string_view string);
	void parse_settings_from_file(std::string_view path);
	std::string parse_setting_in_string();
	void parse_settings_in_file(std::string_view path);

	void init_config_system();
	
	//internal function
	void update_settings();
}

