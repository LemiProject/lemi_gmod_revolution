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
			void exchange(const std::vector<std::string>& c);
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

		enum class e_player_bones
		{
			none = 0,
			head = (1 << 0),
			neck = (1 << 1),
			top_body = (1 << 2),
			body = (1 << 3),
			pelvis = (1 << 4),
			last = pelvis
		}; std::string to_string(e_player_bones e);

		inline std::map<int, std::string> bones_hl_names
		{
			{(1 << 0), "ValveBiped.Bip01_Head1"},
			{(1 << 1), "ValveBiped.Bip01_Neck1"},
			{(1 << 2), "ValveBiped.Bip01_Spine3"},
			{(1 << 3), "ValveBiped.Bip01_Spine"},
			{(1 << 4), "ValveBiped.Bip01_Pelvis"},
		};
	}

	namespace hvh
	{
		enum class e_pitch
		{
			none,
			up,
			down,
			emotional,
			last = emotional
		};
		//Sorry i'm lazy hd))
		inline const char* to_string(e_pitch e)
		{
			switch (e)
			{
			case e_pitch::none: return "";
			case e_pitch::up: return "Up";
			case e_pitch::down: return "Down";
			case e_pitch::emotional: return "Emotional";
			default: return "";
			}
		}

		enum class e_yaw
		{
			none,
			forward,
			backward,
			last = backward
		};
		inline const char* to_string(e_yaw e)
		{
			switch (e)
			{
			case e_yaw::none: return "";
			case e_yaw::forward: return "Forward";
			case e_yaw::backward: return "Backward";
			default: return "";
			}
		}
		
	}
	
	namespace other
	{
		inline std::vector<std::string> friends;
		inline std::vector<int> friendly_teams;

		inline bool load_and_save_style;
		inline bool load_bypass = true;

		inline bool legit_aa = false;
	}

	inline std::map<std::string, bool> states {
		// Player ESP
		{"visuals::esp_enabled_player", false},
		{"visuals::esp_global", false},
		{"visuals::esp_box_player", false},
		{"visuals::esp_name_player", false},
		{"visuals::esp_team_player", false},
		{"visuals::esp_armor_player", false},
		{"visuals::esp_health_player", false},
		{"visuals::esp_health_text_player", false},
		{"visuals::esp_active_weapon_player", false},
		{"visuals::esp_color_by_team_player", false},
		{"visuals::esp_player_user_group", false},
		{"visuals::esp_player_dormant", false},
		
		// Entity ESP
		{"visuals::esp_enabled_entity", false},
		{"visuals::esp_box_entity", false},
		{"visuals::esp_name_entity", false},
		{"visuals::esp_health_entity", false},
		{"visuals::esp_health_text_entity", false},
		{"visuals::esp_entity_dormant", false},

		// Chams
		{"visuals::chams_enabled", false},
		{"visuals::entity_chams_enabled", false},
		{"visuals::ignore_z", false},
		{"visuals::chams_obs_check", false},
		
		// Aimbot
		{"aim_bot::aim_bot_enabled", false},
		{"aim_bot::aim_bot_silent_aim", false},
		{"aim_bot::no_recoil", false},
		{"aim_bot::no_spread", false},
		{"visuals::draw_fov", false},
		{"visuals::draw_line_to_target", false},
		{"aim_bot::aim_bot_auto_fire", false},
		{"aim_bot::aim_bot_trigger_bot", false},
		
		// Misc
		{"misc::bunny_hop", false},
		{"misc::fix_movement", false},
		{"other::anti_obs", false},
		{"other::rapid_fire", false},
		{"misc::auto_strafe", false},

		//World visuals
		{"world::third_person", false},
		{"world::free_camera", false},
		
		// Lua
		{"lua::hack_hooks", false},
		{"lua::hack_globals", false},

		//HVH
		{"hvh::anti_aims", false},
		{"hvh::fake_lags", false},
	};
	
	inline std::map<std::string, float> values {
		// Player ESP
		{"visuals::esp_distance_player", 20000.f},
		{"visuals::esp_box_type_player", (float)visuals::e_esp_box_type::bounding},

		// Entity ESP
		{"visuals::esp_distance_entity", 20000.f},
		{"visuals::esp_box_type_entity", (float)visuals::e_esp_box_type::bounding},

		// Aimbot
		{"aim_bot::aim_bot_fov", 10.f},
		{"aim_bot::aim_bot_smooth_value", 0.f},
		{"aim_bot::aim_bot_delay_before_aiming", 0.f},

		//World visuals
		{"world::third_person_distance", 0.f},
		{"world::free_camera_speed", 1.f}, //1 - 10
		{"world::fov", 0.f},
		
		//HVH
		{"hvh::yaw_type", 0},
		{"hvh::pitch_type", 0},
		{"hvh::fake_lags_value", 0}
	};
	
	inline std::map<std::string, std::string> strings {
		{"visuals::chams_material", "debug/debugambientcube"}
	};
	
	inline std::map<std::string, uint32_t> binds {
		// Misc
		{"exploits::wallpush", 0},
		{"exploits::auto_mega_jump", 0},
		{"other::menu_key", 0},
		{"other::add_entity", 0},

		// Aimbot
		{"aim_bot::aim_bot_key", 0},
		{"aim_bot::aim_bot_auto_fire_key", 0},
		{"aim_bot::aim_bot_trigger_bot_key", 0},

		//WORLD
		{"world::third_person_key", 0},
		{"world::free_camera_key", 0},
		
		//HVH
		{"hvh::fake_duck", 0}
	};

	inline std::map<std::string, int> flags {
		{"aim_bot::aim_bot_player_filter", 0},
		{"aim_bot::aim_bot_player_bones", 0}
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

	bool get_bind_state(const std::string& name, bool may_be_null = true);
	bool get_bind_state(uint32_t bind, bool may_be_null = true);
	
	namespace lua_api
	{
		LUA_FUNCTION(lua_api_get_hack_var);
		LUA_FUNCTION(lua_api_is_key_down);
		
		void push_all(c_lua_interface* lua);
	}

#ifdef _DEBUG
	namespace debug_vars
	{
		//bool no_fall_damage = false;
		inline uint32_t no_fall_key = 0;
		
	}
#endif

}

