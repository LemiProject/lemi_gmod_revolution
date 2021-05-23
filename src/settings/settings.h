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

	namespace aimbot
	{
		enum class e_player_filter
		{
			none,
			noclip = (1 << 0),
			fly = (1 << 1),
			admin = (1 << 2),
			observer = (1 << 3),
			last = observer
		}; const char* to_string(e_player_filter);
	}

	namespace other
	{
		inline std::vector<std::string> friends;
		inline std::vector<int> friendly_teams;
	}

	inline std::map<std::string, bool> states {
		// Player ESP
		{"visuals::esp_enabled_player", false},
		{"visuals::esp_global", false},
		{"visuals::esp_box_player", false},
		{"visuals::esp_name_player", false},
		{"visuals::esp_armor_player", false},
		{"visuals::esp_health_player", false},
		{"visuals::esp_health_text_player", false},
		{"visuals::esp_active_weapon_player", false},
		{"visuals::esp_color_by_team_player", false},
		{"visuals::esp_player_user_group", false},
		
		// Entity ESP
		{"visuals::esp_enabled_entity", false},
		{"visuals::esp_box_entity", false},
		{"visuals::esp_name_entity", false},
		{"visuals::esp_health_entity", false},
		{"visuals::esp_health_text_entity", false},

		// Chams
		{"visuals::chams_enabled", false},
		{"visuals::entity_chams_enabled", false},
		{"visuals::ignore_z", false},
		{"visuals::chams_obs_check", false},
		
		// Legitbot
		{"legit_bot::legit_bot_enabled", false},
		{"legit_bot::legit_bot_silent_aim", false},
		{"legit_bot::no_recoil", false},
		{"legit_bot::no_spread", false},
		{"visuals::draw_fov", false},
		{"visuals::draw_line_to_target", false},
		{"legit_bot::legit_bot_auto_fire", false},
		
		// Misc
		{"misc::bunny_hop", false},
		{"misc::fix_movement", false},
		{"other::anti_obs", false},
		{"misc::auto_strafe", false},

		// Lua
		{"lua::hack_hooks", false},
		{"lua::hack_globals", false},
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
		{"other::add_entity", 0},

		// Legitbot
		{"legit_bot::legit_bot_key", 0},
		{"legit_bot::legit_bot_auto_fire_key", 0}
	};

	inline std::map<std::string, int> flags {
		{"legit_bot::legit_bot_player_filter", 0},
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
			{"esp_armor_color_hp", {1, 1, 1, 0.8f}},
			{"esp_armor_color_void", {0, 0, 0, 0}},
			{"esp_usergoup_text_user", {1, 1, 1, 1}},
			{"esp_usergoup_text_admin", {1, 0, 0, 1}},

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

	namespace lua_api
	{
		LUA_FUNCTION(lua_api_get_hack_var);
		LUA_FUNCTION(lua_api_is_key_down);
		
		void push_all(c_lua_interface* lua);
	}
}

