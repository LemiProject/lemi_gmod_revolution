#include "tabs_contents.h"
#include "../../../settings/settings.h"
#include "../../../render_system/render_system.h"

#include <imgui/imgui_stdlib.h>

#include <fmt/format.h>

#include <imgui/im_tools.h>

#include "file_tools.h"

#include <fstream>

#include "../../../utils/hack_utils.h"

using namespace ImGui;
using namespace settings;

namespace menu_tabs_content::internal
{
	inline ImGuiComboFlags combo_flags = ImGuiComboFlags_NoArrowButton;

	inline ImVec2 calc_toggle_button_size()
	{
		return { GetWindowSize().x / 12, GetWindowSize().y / 25 };
	}

	inline void text_and_toggle_button(const std::string& text, const std::string& tag, bool* var)
	{
		Text("%s", text.c_str());
		SameLine();
		ToggleButton(tag.c_str(), var);
	}

	inline void set_tooltip(const std::string& text, ...)
	{
		if (IsItemHovered())
		{
			auto fmt = text.c_str();
			BeginTooltip();
				va_list args;
				va_start(args, fmt);
					TextV(fmt, args);
				va_end(args);
			EndTooltip();
		}
	}
}

void menu_tabs_content::draw_aim_bot()
{
	constexpr auto panels_in_visuals_count = 3;
	BeginGroupPanel("Aimbot##Aimbot_MAIN", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##Aimbot_MAIN_ENABLED", &states["aim_bot::aim_bot_enabled"]);
		internal::set_tooltip("Enable Aimbot");

		Hotkey("Keybind##Aimbot_MAIN_TOGGLEKEY", &binds["aim_bot::aim_bot_key"], { 0, 0 });
		internal::set_tooltip("Key to toggle aimbotting");

		internal::text_and_toggle_button("Autofire", "##Aimbot_AUTOFIRE_ENB", &states["aim_bot::aim_bot_auto_fire"]);
		internal::set_tooltip("Fire when target is can be shooted");

		if (states["aim_bot::aim_bot_auto_fire"])
			SameLine(), Hotkey("Keybind##Aimbot_AUTOFIRE_KEYBIND", &binds["aim_bot::aim_bot_auto_fire_key"]);
		
		SliderFloat("Aimbot fov##Aimbot_MAIN_FOV", &values["aim_bot::aim_bot_fov"], 1.f, 360.f);
		internal::set_tooltip("Degrees from the crosshair where players will be targetted");
		
		SliderFloat("Aimbot smooth##Aimbot_MAIN_SMOOTH_VAL", &values["aim_bot::aim_bot_smooth_value"], 0.f, 100.f);
		internal::set_tooltip("Aimbot smoothing value");
		
		SliderFloat("Aimbot delay##Aimbot_MAIN_DELAY", &values["aim_bot::aim_bot_delay_before_aiming"], 0.f, 10000.f, "%.0f");
		internal::set_tooltip("Delay before shooting");

		PushItemWidth(GetWindowSize().x / 6);
		if (BeginCombo("##aim_bot_BONES", "Bones", internal::combo_flags))
		{
			for (auto i = 1; i <= (int)aimbot::e_player_bones::last; ++i)
			{
				if (!(to_string((aimbot::e_player_bones)i).empty()))
				{
					auto selected = flags["aim_bot::aim_bot_player_bones"] & i ? true : false;
					CheckboxFlags(to_string((aimbot::e_player_bones)i).c_str(), &flags["aim_bot::aim_bot_player_bones"], i);
				}
			}
			EndCombo();
		}
		internal::set_tooltip("The bones that will work aimbot");
		
		SameLine();
		
		if (BeginCombo("##aim_bot_IGNORE_FLAGS", "IngnoreFlags", internal::combo_flags))
		{
			for (auto i = 1; i <= (int)aimbot::e_player_filter::last; ++i)
			{				
				if (!(std::string(to_string((aimbot::e_player_filter)i)) == "unk"))
				{
					auto selected = flags["aim_bot::aim_bot_player_filter"] & i ? true : false;
					CheckboxFlags(to_string((aimbot::e_player_filter)i), &flags["aim_bot::aim_bot_player_filter"], i);
				}
			}
			EndCombo();
		}
		internal::set_tooltip("Aimbot filter");
		
		PopItemWidth();
		
		internal::text_and_toggle_button("Silent", "##Aimbot_MAIN_SILENT", &states["aim_bot::aim_bot_silent_aim"]);
		internal::set_tooltip("Client-Side silent-aim");

		internal::text_and_toggle_button("Rapid fire", "##AIMBOT_RAPIDFIRE", &states["other::rapid_fire"]);
		internal::set_tooltip("Shooting by one bullet");
	}
	EndGroupPanel();

	SameLine();

	BeginGroupPanel("Accuracy##Aimbot_ACCURACY", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("NoRecoil", "##Aimbot_ACCURACY_NORECOIL", &states["aim_bot::no_recoil"]);
		internal::set_tooltip("WARNING! Can cause ban\nDisables recoil on most packs");
		internal::text_and_toggle_button("NoSpread", "##Aimbot_ACCURACY_NOSPREAD", &states["aim_bot::no_spread"]);
		internal::set_tooltip("WARNING! Can cause ban\nAt this time only M9K works, more pack will be added in next few updates");
	}
	EndGroupPanel();

	BeginGroupPanel("Other##Aimbot_OTHER", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Trigger", "##Aimbot_OTHER_TRIGGER", &states["aim_bot::aim_bot_trigger_bot"]);
		internal::set_tooltip("Shooting if enemy on crosshair");
		if (states["aim_bot::aim_bot_trigger_bot"])
			SameLine(), Hotkey("Bind##TRIGGER_BOT_BIND", &binds["aim_bot::aim_bot_trigger_bot_key"]);
		
	}
	EndGroupPanel();
}

void menu_tabs_content::draw_hvh()
{
#ifdef _DEBUG
	constexpr auto panels_in_visuals_count = 3;

	BeginGroupPanel("AnitAim##HVH_AA", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("AntiAim", "##HVH_AA_TOGGLE", &states["hvh::anti_aims"]);
		internal::set_tooltip("Save your head on hvh");

		auto pitch_type_str = std::string(to_string((hvh::e_pitch)values["hvh::pitch_type"]));
		if (BeginCombo("##PITCH_TYPE_COMBO", fmt::format("Pitch type: {}", pitch_type_str).c_str()))
		{
			for (auto i = 1; i <= (int)hvh::e_pitch::last; ++i)
			{
				auto selected = values["hvh::pitch_type"] == i;
				if (Selectable((to_string((hvh::e_pitch)i) + std::string("##") + std::to_string(i)).c_str(), selected))
					values["hvh::pitch_type"] = i;
				if (selected)
					SetItemDefaultFocus();
			}
			EndCombo();
		}
		
		SameLine();

		auto yaw_type_str = std::string(to_string((hvh::e_yaw)values["hvh::yaw_type"]));
		if (BeginCombo("##YAW_TYPE_COMBO", fmt::format("Yaw type: {}", yaw_type_str).c_str()))
		{
			for (auto i = 1; i <= (int)hvh::e_yaw::last; ++i)
			{
				auto selected = values["hvh::yaw_type"] == i;
				if (Selectable((to_string((hvh::e_yaw)i) + std::string("##") + std::to_string(i)).c_str(), selected))
					values["hvh::yaw_type"] = i;
				if (selected)
					SetItemDefaultFocus();
			}
			EndCombo();
		}
	}
	EndGroupPanel();
#endif
}

void menu_tabs_content::draw_visuals()
{
	constexpr auto panels_in_visuals_count = 3;

	BeginGroupPanel("Player ESP##VISUALS_ESP_PLAYER", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##VISUALS_ESP_PLAYER_ENABLED", &states["visuals::esp_enabled_player"]);
		internal::set_tooltip("Esp enabled");
		internal::text_and_toggle_button("Global ESP", "##VISUALS_ESP_GLOBAL_ENABLED", &states["visuals::esp_global"]);
		internal::set_tooltip("ESP settings for entities and players");
		internal::text_and_toggle_button("Box enabled", "##VISUALS_ESP_PLAYER_BOX_ENABLE", &states["visuals::esp_box_player"]);
		internal::set_tooltip("Draw box");
		if (settings::states["visuals::esp_box_player"])
		{
			internal::text_and_toggle_button("Box team color", "##VISUALS_ESP_PLAYER_COLOR_BY_TEAM", &states["visuals::esp_color_by_team_player"]);
			internal::set_tooltip("Use the player's team color for rendering");

			if (BeginCombo("##VISUALS_ESP_PLAYER_BOX_TYPE_COMBO", fmt::format("Box type: {}", to_string(static_cast<visuals::e_esp_box_type>(values["visuals::esp_box_type_player"]))).c_str(), internal::combo_flags))
			{
				for (auto i = 0; i < static_cast<int>(visuals::e_esp_box_type::iter_last); ++i)
				{
					const auto is_selected = (values["visuals::esp_box_type_player"] == i);
					if (Selectable(to_string(static_cast<visuals::e_esp_box_type>(i)), is_selected))
						values["visuals::esp_box_type_player"] = i;
					if (is_selected)
						SetItemDefaultFocus();
				}
				EndCombo();
			}
		}

		internal::text_and_toggle_button("Name", "##VISUALS_ESP_PLAYER_NAME_ENABLE", &states["visuals::esp_name_player"]);
		internal::set_tooltip("Draw entity's name");
		internal::text_and_toggle_button("Health", "##VISUALS_ESP_PLAYER_HEALTH_ENABLE", &states["visuals::esp_health_player"]);
		internal::set_tooltip("Draw entity's health");
		SameLine();
		internal::text_and_toggle_button("Text", "##VISUALS_ESP_PLAYER_HEALTH_TEXT_ENABLE", &states["visuals::esp_health_text_player"]);
		internal::set_tooltip("Draw entity's health text");
		internal::text_and_toggle_button("User group", "##VISUALS_ESP_PLAYER_DRAW_USER_GROUP", &states["visuals::esp_player_user_group"]);
		internal::set_tooltip("Draw player user group");
		internal::text_and_toggle_button("Armor", "##VISUALS_ESP_PLAYER_ARMOR_ENABLE", &states["visuals::esp_armor_player"]);
		internal::set_tooltip("Draw entity's armor");
		internal::text_and_toggle_button("Active weapon", "##VISUALS_ESP_PLAYER_ACTIVEWEAPON_NAME", &states["visuals::esp_active_weapon_player"]);
		internal::set_tooltip("Draw active weapon name");
		SliderFloat("ESP Draw distance##VISUALS_ESP_PLAYER_DRAW_DISTANCE", &values["visuals::esp_distance_player"], 0.f, 20000.f, "%.1f", 1.f);
		internal::set_tooltip("Distance from target until ESP stops rendering");
	}
	EndGroupPanel();

	SameLine();

	BeginGroupPanel("Chams##VISUALS_CHAMS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##VISUALS_CHAMS_ENABLED", &states["visuals::chams_enabled"]);
		internal::set_tooltip("Draw model with custom material");
		internal::text_and_toggle_button("Anti-OBS Check", "##VISUALS_CHAMS_OBSCHECK", &states["visuals::chams_obs_check"]);
		internal::set_tooltip("Disable chams when Anti-OBS is enabled");
		internal::text_and_toggle_button("IgnoreZ", "##VISUALS_CHAMS_IGNOREZ",  &states["visuals::ignore_z"]);
		internal::set_tooltip("Draw models thru walls");
		internal::text_and_toggle_button("Entity chams", "##VISUALS_CHAMS_ENTITY", &states["visuals::entity_chams_enabled"]);
		internal::set_tooltip("Apply chams on entitys");
		
		InputText("Material##VISUALS_CHAMS_MATERIAL", &strings["visuals::chams_material"]);
		internal::set_tooltip("Gmod chams material");
	}
	EndGroupPanel();
	
	if (!states["visuals::esp_global"])
	{
		BeginGroupPanel("Entity ESP##VISUALS_ESP_ENTITY", { GetWindowSize().x / panels_in_visuals_count, -1 });
		{
			internal::text_and_toggle_button("Enabled", "##VISUALS_ESP_ENTITY_ENABLED", &states["visuals::esp_enabled_entity"]);
			internal::set_tooltip("Esp enabled");
			internal::text_and_toggle_button("Box enabled", "##VISUALS_ESP_ENTITY_BOX_ENABLE", &states["visuals::esp_box_entity"]);
			internal::set_tooltip("Draw box");
			if (states["visuals::esp_box_entity"])
			{
				if (BeginCombo("##VISUALS_ESP_ENTITY_BOX_TYPE_COMBO", fmt::format("Box type: {}", to_string(static_cast<visuals::e_esp_box_type>(values["visuals::esp_box_type_entity"]))).c_str(), internal::combo_flags))
				{
					for (auto i = 0; i < static_cast<int>(visuals::e_esp_box_type::iter_last); ++i)
					{
						const auto is_selected = (values["visuals::esp_box_type_entity"] == i);
						if (Selectable(to_string(static_cast<visuals::e_esp_box_type>(i)), is_selected))
							values["visuals::esp_box_type_entity"] = i;
						if (is_selected)
							SetItemDefaultFocus();
					}
					EndCombo();
				}
			}

			internal::text_and_toggle_button("Name", "##VISUALS_ESP_ENTITY_NAME_ENABLE", &states["visuals::esp_name_entity"]);
			internal::set_tooltip("Draw entity's name");
			internal::text_and_toggle_button("Health", "##VISUALS_ESP_ENTITY_HEALTH_ENABLE", &states["visuals::esp_health_entity"]);
			internal::set_tooltip("Draw entity's health");
			SameLine();
			internal::text_and_toggle_button("Health text", "##VISUALS_ESP_ENTITY_HEALTH_TEXT_ENABLE", &states["visuals::esp_health_text_entity"]);
			internal::set_tooltip("Draw entity's health text");
			SliderFloat("ESP Draw distance##VISUALS_ESP_ENTITY_DRAW_DISTANCE", &values["visuals::esp_distance_entity"], 0.f, 20000.f, "%.1f", 1.f);
			internal::set_tooltip("Distance from target until ESP stops rendering");
		}
		EndGroupPanel();

		SameLine();
	}

	BeginGroupPanel("Overlay##VISUALS_OVERLAY", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Draw fov", "##VISUALS_OVERLAY_DRAW_FOV", &states["visuals::draw_fov"]);
		internal::set_tooltip("Draw aimbot fov");
		//internal::text_and_toggle_button("Line to target", "##VISUALS_OVERLAY_DRAW_LINE_TO_LB_TARGET", &states["visuals::draw_line_to_target"]);
		//internal::set_tooltip("Draw line to Aimbot target");
	}
	EndGroupPanel();
}

void menu_tabs_content::draw_misc()
{
	constexpr auto panels_in_visuals_count = 3;

	BeginGroupPanel("Movement##MOVEMENT", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Bunny Hop", "##MOVEMENT_BHOP", &states["misc::bunny_hop"]);
		internal::set_tooltip("Not 'perfect' bhop");

		internal::text_and_toggle_button("Auto strafe", "##MOVEMENT_BHOP_AUTO_STRAFE", &states["misc::auto_strafe"]);
		internal::set_tooltip("Not 'perfect' auto strafe for bhop");
		
		internal::text_and_toggle_button("Fix movement", "##MOVEMENT_FIX_MOVEMENT", &states["misc::fix_movement"]);
		internal::set_tooltip("WARNING! Can cause ban\nFix your movement for correct silent aim");
	}
	EndGroupPanel();

	BeginGroupPanel("Exploits##EXPLOITS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		Hotkey("Wallpush##EXPLOITS_WALLPUSH", &binds["exploits::wallpush"], { 0, 0 });
		internal::set_tooltip("Worked only on servers with 'sit' command");

		Hotkey("Auto mega-jump", &binds["exploits::auto_mega_jump"]);
		internal::set_tooltip("Automatically places a block under you, which on most servers allows you to take off high");
	}
	EndGroupPanel();
}

void menu_tabs_content::draw_setting()
{
	constexpr auto panels_in_visuals_count = 3;
	BeginGroupPanel("Main##SETTINGS_PANEL", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Anti-OBS", "##SETTINGS_ANTIOBS", &states["other::anti_obs"]);
		internal::set_tooltip("At this time can cause ESP rip");
		Hotkey("Menu key##SETTINGS_PANEL", &binds["other::menu_key"], { 0 ,0 }, VK_INSERT, "Insert");
		internal::set_tooltip("Button that opens lemiproject menu");
	}
	EndGroupPanel();

	SameLine();
	
	BeginGroupPanel("Configs##SETTINGS_CONFIGS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		static auto current_cfg = -1;
		static std::vector<std::string> last_configs;
		static std::string new_cfg_name = "cfg";
		
		if (last_configs.empty())
			last_configs = get_configs();
		
		if (BeginListBox("Configs##SETTINGS_CONFIGS_LIST", { 0, 100 }))
		{
			auto cfgs = last_configs;
			for (auto i = 0; i < cfgs.size(); ++i)
			{
				if (Selectable((cfgs[i] + "##SETTINGS_CONFIGS_LIST_ITEM").c_str(), i == current_cfg))
					current_cfg = i;
				if (i == current_cfg)
					SetItemDefaultFocus();
			}
			EndListBox();
		}
		
		if (Button("Save##SAVE_SETTINGS") && current_cfg != -1)
			std::thread(parse_settings_in_file, config_directory() + "\\" + last_configs[current_cfg]).detach();
		SameLine();
		if (Button("Load##LOAD_SETTINGS") && current_cfg != -1)
			std::thread(parse_settings_from_file, config_directory() + "\\" + last_configs[current_cfg]).detach();
		SameLine();
		if (Button("Reload##SETTINGS_RELOAD"))
			last_configs = get_configs();
		
		InputText("Name##NEW_CFG_NAME", &new_cfg_name);
		auto its = GetItemRectSize();
		SameLine();
		if (Button("Add##SETTINGS_ADD"))
		{
			try
			{
				auto path = config_directory() + "\\" + new_cfg_name + ".vpcfg";
				if (!file_tools::exist(path))
				{
					{
						std::ofstream s(path);
						s << "\0";
					}
					last_configs = get_configs();
					std::thread(parse_settings_in_file, path).detach();
					current_cfg = std::distance(last_configs.begin(), std::find(last_configs.begin(), last_configs.end(), new_cfg_name + ".vpcfg"));
				}
			}
			catch (std::exception& e)
			{
				auto msg = MessageBox(render_system::vars::game_hwnd, fmt::format("Failed to add config with error:\n{}", e.what()).c_str(), "LemiProject", MB_OK);
				hack_utils::shutdown_hack();
			}
		}

		if (Button("Open directory##OPEN_DIRECTORY"))
			ShellExecute(NULL, NULL, config_directory().c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	EndGroupPanel();
	
}
