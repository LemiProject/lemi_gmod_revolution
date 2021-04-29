#pragma once

#include <imgui/im_tools.h>
#include <imgui/imgui_stdlib.h>

#include "../../../settings/settings.h"

#include <fmt/core.h>

#include "../../../render_system/render_system.h"

namespace menu_tabs_content
{
	using namespace ImGui;
	using namespace settings;


	namespace internal
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
	
	inline void draw_legit_bot()
	{
		constexpr auto panels_in_visuals_count = 3;
		BeginGroupPanel("Legitbot##LEGITBOT_MAIN", { GetWindowSize().x / panels_in_visuals_count, -1 });
		{
			internal::text_and_toggle_button("Enabled", "##LEGITBOT_MAIN_ENABLED", &aim::legit_bot_enabled);
			SliderFloat("Legitbot fov##LEGITBOT_MAIN_FOV", &aim::legit_bot_fov, 1.f, 360.f);
		}
		EndGroupPanel();

		SameLine();

		BeginGroupPanel("Accuracy##LEGITBOT_ACCURACY", { GetWindowSize().x / panels_in_visuals_count, -1 });
		{
			internal::text_and_toggle_button("RCS", "##LEGITBOT_ACCURACY_RCS", &aim::rcs_standalone);
		}
		EndGroupPanel();
	}

	inline void draw_rage_bot()
	{
	}

	inline void draw_visuals()
	{
		constexpr auto panels_in_visuals_count = 3;
		
		BeginGroupPanel("ESP##VISUALS_ESP", {GetWindowSize().x / panels_in_visuals_count, -1});
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

	inline void draw_misc()
	{
		constexpr auto panels_in_visuals_count = 3;
		
		BeginGroupPanel("Movement##MOVEMENT", { GetWindowSize().x / panels_in_visuals_count, -1 });
		{
			internal::text_and_toggle_button("Bunny Hop", "##MOVEMENT_BHOP", &misc::bunny_hop);
		}
		EndGroupPanel();
		
	}

	inline void draw_setting()
	{
	}
}
