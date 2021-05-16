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
		
		//inline auto esp = true;
		//inline auto esp_box = true;
		//inline auto esp_name = true;
		//inline auto esp_health = true;
		//inline auto esp_active_weapon = true;
		//
		//inline auto esp_draw_distance = 100000.f;
		//
		//inline auto esp_box_color_by_team = false;
		//
		//inline int esp_box_type = static_cast<int>(e_esp_box_type::bounding);

		
		inline c_entity_list entitys_to_draw;

		//inline auto chams = true;
		//inline auto entity_chams = true;
		//inline auto ignore_z = true;
		//inline std::string chams_material = "debug/debugambientcube";

		//inline bool draw_fov = false;
	}

	namespace aim
	{
		//inline bool legit_bot_enabled = false;
		//inline float legit_bot_fov = 30.f;
		//
		//inline float legit_bot_smooth_val = 0.f;
		//
		//inline bool no_recoil = false;
		//inline bool no_spread = false;

		//inline bool legit_bot_silent_aim = true;
		//
		//inline float legit_bot_delay_before_aiming = 0.f;
	}

	namespace misc
	{
		//inline bool bunny_hop = true;

		//inline bool fix_movement = false;
		//
		//namespace exploits
		//{
		//	inline uint32_t wallpush = 0;
		//}
		
	}

	namespace other
	{
		//inline uint32_t menu_key = 0;
		//inline bool anti_obs = false;

		inline std::vector<std::string> friends;
		inline std::vector<int> friendly_teams;
	}

	inline std::map<std::string, bool> states {
		{"visuals::esp_enabled", true},
		{"visuals::esp_draw_box", true},
		{"visuals::esp_draw_name", true},
		{"visuals::esp_draw_health", true},
		{"visuals::esp_draw_active_weapon", true},
		{"visuals::esp_color_by_team", true},
		{"visuals::chams_enabled", true},
		{"visuals::entity_chams_enabled", true},
		{"visuals::ignore_z", true},
		{"visuals::draw_fov", false},
		
		{"legit_bot::legit_bot_enabled", false},
		{"legit_bot::legit_bot_silent_aim", false},
		{"legit_bot::no_recoil", false},
		{"legit_bot::no_spread", false},
		
		{"misc::bunny_hop", true},
		{"misc::fix_movement", false},
		
		{"other::anti_obs", false},
	};
	
	inline std::map<std::string, float> values {
		{"visuals::esp_draw_distance", 100000.f},
		{"visuals::esp_box_type", (float)visuals::e_esp_box_type::bounding},

		{"legit_bot::legit_bot_fov", 10.f},
		{"legit_bot::legit_bot_smooth_value", 0.f},
		{"legit_bot::legit_bot_delay_before_aiming", 0.f},
	};
	
	inline std::map<std::string, std::string> strings {
		{"visuals::chams_material", "debug/debugambientcube"}
	};
	
	inline std::map<std::string, uint32_t> binds {
		{"exploits::wallpush", 0},
		{"other::menu_key", 0},
	};
	namespace colors
	{
		typedef std::array<float, 4> color_t;
		inline std::map<std::string, color_t> colors_map
		{
			{"esp_box_color", {1, 0, 0, 0.8f}},
			{"esp_health_color_hp", {0, 1, 0, 0.8f}},
			{"esp_health_color_void", {0, 0, 0, 0}},
			{"esp_weapon_name_color", {1.f, 1.f, 1.f, 1.f}},
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

