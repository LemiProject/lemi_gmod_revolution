#pragma once
#include <any>
#include <string>
#include <json.hpp>
#include <vector>
#include <map>
#include <mutex>

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

		class c_entity_list
		{
			std::vector<std::string> classes;
			std::mutex mutex;
			
		public:
			void push_back(const std::string& c);
			void remove(int idx);
			bool exist(const std::string& c);
			int find(const std::string& c);
			void exchange(std::vector<std::string>& c);
			bool empty();
			void clear();

			size_t size();
			std::vector<std::string> data();
		};

		inline c_entity_list entitys_to_draw;
	}

	namespace other
	{
		inline std::vector<std::string> friends;
		inline std::vector<int> friendly_teams;
	}

	inline std::map<std::string, bool> states {
		// Player ESP
		{"visuals::esp_enabled_player", true},
		{"visuals::esp_global", true},
		{"visuals::esp_box_player", true},
		{"visuals::esp_name_player", true},
		{"visuals::esp_health_player", true},
		{"visuals::esp_active_weapon_player", true},
		{"visuals::esp_color_by_team_player", true},

		// Entity ESP
		{"visuals::esp_enabled_entity", true},
		{"visuals::esp_box_entity", true},
		{"visuals::esp_name_entity", true},
		{"visuals::esp_health_entity", true},

		// Chams
		{"visuals::chams_enabled", true},
		{"visuals::entity_chams_enabled", true},
		{"visuals::ignore_z", true},
		{"visuals::chams_obs_check", true},
		
		// Legitbot
		{"legit_bot::legit_bot_enabled", false},
		{"legit_bot::legit_bot_silent_aim", false},
		{"legit_bot::no_recoil", false},
		{"legit_bot::no_spread", false},
		{"visuals::draw_fov", false},
		{"visuals::draw_line_to_target", false},
		
		// Misc
		{"misc::bunny_hop", true},
		{"misc::fix_movement", false},
		{"other::anti_obs", false},
	};
	
	inline std::map<std::string, float> values {
		// Player ESP
		{"visuals::esp_distance_player", 20000.f},
		{"visuals::esp_box_type_player", (float)visuals::e_esp_box_type::bounding},

		// Entity ESP
		{"visuals::esp_distance_entity", 20000.f},
		{"visuals::esp_box_type_entity", (float)visuals::e_esp_box_type::bounding},

		// Legitbot
		{"legit_bot::legit_bot_fov", 10.f},
		{"legit_bot::legit_bot_smooth_value", 0.f},
		{"legit_bot::legit_bot_delay_before_aiming", 0.f},
	};
	
	inline std::map<std::string, std::string> strings {
		{"visuals::chams_material", "debug/debugambientcube"}
	};
	
	inline std::map<std::string, uint32_t> binds {
		// Misc
		{"exploits::wallpush", 0},
		{"other::menu_key", 0},

		// Legitbot
		{"legit_bot::legit_bot_key", 0},
	};
	namespace colors
	{
		typedef std::array<float, 4> color_t;
		inline std::map<std::string, color_t> colors_map
		{
			// ESP
			{"esp_box_color", {1, 0, 0, 0.8f}},
			{"esp_health_color_hp", {0, 1, 0, 0.8f}},
			{"esp_health_color_void", {0, 0, 0, 0}},
			{"esp_weapon_name_color", {1.f, 1.f, 1.f, 1.f}},

			// Chams
			{"chams_color_modulation", {0, 0.7f, 0.3f, 1}}
		};
	}
	
	void parse_settings_from_string(const std::string& s);
	void parse_settings_from_file(const std::string& s);
	std::string parse_setting_in_string();
	void parse_settings_in_file(const std::string& path);

	std::string config_directory();
	std::vector<std::string> get_configs();
	
	void init_config_system();
}

