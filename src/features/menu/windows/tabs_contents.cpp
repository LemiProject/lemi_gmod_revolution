#include "tabs_contents.h"
#include "../../../settings/settings.h"
#include "../../../render_system/render_system.h"

#include <imgui/imgui_stdlib.h>

#include <fmt/format.h>

#include <imgui/im_tools.h>

#include "file_tools.h"

#include <fstream>

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

void menu_tabs_content::draw_legit_bot()
{
	constexpr auto panels_in_visuals_count = 3;
	BeginGroupPanel("Legitbot##LEGITBOT_MAIN", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##LEGITBOT_MAIN_ENABLED", &states["legit_bot::legit_bot_enabled"]);
		internal::set_tooltip("Enable legitbot");
		
		SliderFloat("Legitbot fov##LEGITBOT_MAIN_FOV", &values["legit_bot::legit_bot_fov"], 1.f, 360.f);
		internal::set_tooltip("Distance to crosshair at which it will work legitbot");
		
		SliderFloat("Legitbot smooth##LEGITBOT_MAIN_SMOOTH_VAL", &values["legit_bot::legit_bot_smooth_value"], 0.f, 100.f);
		internal::set_tooltip("Legitbot smoothing value");
		
		SliderFloat("Legitbot delay##LEGITBOT_MAIN_DELAY", &values["legit_bot::legit_bot_delay_before_aiming"], 0.f, 10000.f, "%.0f");
		internal::set_tooltip("Delay before shoot");
		
		internal::text_and_toggle_button("Silent", "##LEGITBOT_MAIN_SILENT", &states["legit_bot::legit_bot_silent_aim"]);
		internal::set_tooltip("Client-side silent");
	}
	EndGroupPanel();

	SameLine();

	BeginGroupPanel("Accuracy##LEGITBOT_ACCURACY", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("NoRecoil", "##LEGITBOT_ACCURACY_NORECOIL", &states["legit_bot::no_recoil"]);
		internal::set_tooltip("WARNING! Can cause ban\nDisable recoil on most packs");
		internal::text_and_toggle_button("NoSpread", "##LEGITBOT_ACCURACY_NOSPREAD", &states["legit_bot::no_spread"]);
		internal::set_tooltip("WARNING! Can cause ban\nAt this time only M9K, more pack has been done in next updates");
	}
	EndGroupPanel();
}

void menu_tabs_content::draw_rage_bot()
{
}

void menu_tabs_content::draw_visuals()
{
	constexpr auto panels_in_visuals_count = 3;

	BeginGroupPanel("ESP##VISUALS_ESP", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##VISUALS_ESP_ENABLED", &states["visuals::esp_enabled"]);
		internal::set_tooltip("Esp enabled");
		internal::text_and_toggle_button("Box enabled", "##VISUALS_ESP_BOX_ENABLE", &states["visuals::esp_draw_box"]);
		internal::set_tooltip("Draw box");
		internal::text_and_toggle_button("Box team color", "##VISUALS_ESP_COLOR_BY_TEAM", &states["visuals::esp_color_by_team"]);
		internal::set_tooltip("Get team color and using him for drawing");
		if (states["visuals::esp_enabled"])
		{
			if (BeginCombo("##VISUALS_ESP_BOX_TYPE_COMBO", fmt::format("Box type: {}", to_string(static_cast<visuals::e_esp_box_type>(values["visuals::esp_box_type"]))).c_str(), internal::combo_flags))
			{
				for (auto i = 0; i < static_cast<int>(visuals::e_esp_box_type::iter_last); ++i)
				{
					const auto is_selected = (values["visuals::esp_box_type"] == i);
					if (Selectable(to_string(static_cast<visuals::e_esp_box_type>(i)), is_selected))
						values["visuals::esp_box_type"] = i;
					if (is_selected)
						SetItemDefaultFocus();
				}
				EndCombo();
			}
		}

		internal::text_and_toggle_button("Name", "##VISUALS_ESP_NAME_ENABLE", &states["visuals::esp_draw_name"]);
		internal::set_tooltip("Draw entity name");
		internal::text_and_toggle_button("Health", "##VISUALS_ESP_HEALTH_ENABLE", &states["visuals::esp_draw_health"]);
		internal::set_tooltip("Draw entity health");
		internal::text_and_toggle_button("Active weapon", "##VISUALS_ESP_ACTIVEWEAPON_NAME", &states["visuals::esp_draw_active_weapon"]);
		internal::set_tooltip("Draw active weapon name");
		SliderFloat("ESP Draw distance##VISUALS_ESP_DRAW_DISTANCE", &values["visuals::esp_draw_distance"], 0.f, 100000.f, "%.1f", 1.f);
		internal::set_tooltip("Distance to target on which esp will draw");
	}
	EndGroupPanel();

	SameLine();

	BeginGroupPanel("CHAMS##VISUALS_CHAMS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##VISUALS_CHAMS_ENABLED", &states["visuals::chams_enabled"]);
		internal::set_tooltip("Draw model with custom material");
		internal::text_and_toggle_button("IgnoreZ", "##VISUALS_CHAMS_IGNOREZ", &states["visuals::ignore_z"]);
		internal::set_tooltip("Disable draw models through the walls");
		internal::text_and_toggle_button("Entity chams", "##VISUALS_CHAMS_ENTITY", &states["visuals::entity_chams_enabled"]);
		internal::set_tooltip("Apply chams on entitys");
		
		InputText("Material##VISUALS_CHAMS_MATERIAL", &strings["visuals::chams_material"]);
		internal::set_tooltip("Gmod chams material");
	}
	EndGroupPanel();


	BeginGroupPanel("Overlay##VISUALS_OVERLAY", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Draw fov", "##VISUALS_OVERLAY_DRAW_FOV", &states["visuals::draw_fov"]);
		internal::set_tooltip("Draw aimbot fov");
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
		
		internal::text_and_toggle_button("Fix movement", "##MOVEMENT_FIX_MOVEMENT", &states["misc::fix_movement"]);
		internal::set_tooltip("WARNING! Can cause ban\nFix your movement for correct silent aim");
	}
	EndGroupPanel();

	BeginGroupPanel("Exploits##EXPLOITS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		Hotkey("Wallpush##EXPLOITS_WALLPUSH", &binds["exploits::wallpush"], { 0, 0 });
		internal::set_tooltip("Worked only on servers with 'sit' command");
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

		if (Button("Open directory##OPEN_DIRECTORY"))
			system(("explorer " + config_directory()).c_str());
	}
	EndGroupPanel();
	
}
