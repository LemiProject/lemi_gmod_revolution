#include "tabs_contents.h"
#include "../../../settings/settings.h"
#include "../../../render_system/render_system.h"

#include <imgui/imgui_stdlib.h>

#include <fmt/format.h>

#include <imgui/im_tools.h>

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
}

void menu_tabs_content::draw_legit_bot()
{
	constexpr auto panels_in_visuals_count = 3;
	BeginGroupPanel("Legitbot##LEGITBOT_MAIN", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##LEGITBOT_MAIN_ENABLED", &aim::legit_bot_enabled);
		SliderFloat("Legitbot fov##LEGITBOT_MAIN_FOV", &aim::legit_bot_fov, 1.f, 360.f);

		SliderFloat("Legitbot smooth##LEGITBOT_MAIN_SMOOTH_VAL", &aim::legit_bot_smooth_val, 0.f, 100.f);
		
		SliderFloat("Legitbot delay##LEGITBOT_MAIN_DELAY", &aim::legit_bot_delay_before_aiming, 0.f, 10000.f, "%.0f");

		internal::text_and_toggle_button("Silent", "##LEGITBOT_MAIN_SILENT", &aim::legit_bot_silent_aim);
	}
	EndGroupPanel();

	SameLine();

	BeginGroupPanel("Accuracy##LEGITBOT_ACCURACY", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("NoRecoil", "##LEGITBOT_ACCURACY_NORECOIL", &aim::no_recoil);
		internal::text_and_toggle_button("NoSpread", "##LEGITBOT_ACCURACY_NOSPREAD", &aim::no_spread);
		if (IsItemHovered())
		{
			BeginTooltip();
			Text("At this time only M9K, more pack has been done in next updates");
			EndTooltip();
		}
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
		internal::text_and_toggle_button("Enabled", "##VISUALS_ESP_ENABLED", &visuals::esp);
		internal::text_and_toggle_button("Box enabled", "##VISUALS_ESP_BOX_ENABLE", &visuals::esp_box);
		internal::text_and_toggle_button("Box team color", "##VISUALS_ESP_COLOR_BY_TEAM", &visuals::esp_box_color_by_team);
		if (visuals::esp_box)
		{
			if (BeginCombo("##VISUALS_ESP_BOX_TYPE_COMBO", fmt::format("Box type: {}", to_string(static_cast<visuals::e_esp_box_type>(visuals::esp_box_type))).c_str(), internal::combo_flags))
			{
				for (auto i = 0; i < static_cast<int>(visuals::e_esp_box_type::iter_last); ++i)
				{
					const auto is_selected = (visuals::esp_box_type == i);
					if (Selectable(to_string(static_cast<visuals::e_esp_box_type>(i)), is_selected))
						visuals::esp_box_type = i;
					if (is_selected)
						SetItemDefaultFocus();
				}
				EndCombo();
			}
		}

		internal::text_and_toggle_button("Name", "##VISUALS_ESP_NAME_ENABLE", &visuals::esp_name);
		internal::text_and_toggle_button("Health", "##VISUALS_ESP_HEALTH_ENABLE", &visuals::esp_health);
	}
	EndGroupPanel();

	SameLine();

	BeginGroupPanel("CHAMS##VISUALS_CHAMS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Enabled", "##VISUALS_CHAMS_ENABLED", &visuals::chams);
		internal::text_and_toggle_button("IgnoreZ", "##VISUALS_CHAMS_IGNOREZ", &visuals::ignore_z);
		internal::text_and_toggle_button("Entity chams", "##VISUALS_CHAMS_ENTITY", &visuals::entity_chams);

		InputText("Material##VISUALS_CHAMS_MATERIAL", &visuals::chams_material);
	}
	EndGroupPanel();


	BeginGroupPanel("Overlay##VISUALS_OVERLAY", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Draw fov", "##VISUALS_OVERLAY_DRAW_FOV", &visuals::draw_fov);
	}
	EndGroupPanel();
}

void menu_tabs_content::draw_misc()
{
	constexpr auto panels_in_visuals_count = 3;

	BeginGroupPanel("Movement##MOVEMENT", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Bunny Hop", "##MOVEMENT_BHOP", &misc::bunny_hop);
	}
	EndGroupPanel();

	BeginGroupPanel("Exploits##EXPLOITS", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		Hotkey("Wallpush##EXPLOITS_WALLPUSH", &misc::exploits::wallpush, { 0, 0 });
	}
	EndGroupPanel();
}

void menu_tabs_content::draw_setting()
{
	constexpr auto panels_in_visuals_count = 3;
	BeginGroupPanel("Main##SETTINGS_PANEL", { GetWindowSize().x / panels_in_visuals_count, -1 });
	{
		internal::text_and_toggle_button("Anti-OBS", "##SETTINGS_ANTIOBS", &other::anti_obs);
		Hotkey("Menu key##SETTINGS_PANEL", &other::menu_key, { 0 ,0 }, VK_INSERT, "Insert");
	}
	EndGroupPanel();
}
