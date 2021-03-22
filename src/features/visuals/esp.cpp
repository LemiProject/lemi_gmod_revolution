#include <iostream>

#include "visuals.h"

#include "../../utils/game_utils.h"

#include <imgui/im_tools.h>
#include <color.h>

#include <fmt/core.h>


#include "../../render_system/render_system.h"
#include "../../settings/settings.h"


inline ImDrawList* draw_list;

void draw_health(c_base_entity* ent, math::c_box box)
{
	if (ent->is_alive())
	{
		const auto health = std::clamp(ent->get_health(), 0, ent->get_max_health());
		const auto pos = ImVec2(box.x - 6.f, box.y);

		const math::c_box void_box{ pos.x, pos.y, 3, box.h };
		auto health_box{ void_box };

		health_box.h = health * void_box.h / 100;
		health_box.y = void_box.y + void_box.h - health_box.h;

		const auto health_color = c_color(settings::colors::colors_map["esp_health_color_hp"]).get_u32();
		const auto void_color = c_color(settings::colors::colors_map["esp_health_color_void"]).get_u32();
		
		draw_list->AddRectFilled(ImVec2(void_box.x, void_box.y), ImVec2(void_box.x + void_box.w, void_box.y + void_box.h), void_color);
		draw_list->AddRectFilled(ImVec2(health_box.x, health_box.y), ImVec2(health_box.x + health_box.w, health_box.y + health_box.h), health_color);
	}
}

inline void draw_name(c_base_entity* ent, math::c_box& box)
{
	ImGui::PushFont(render_system::fonts::in_game_font);
	
	const auto text = ent->is_player() ? static_cast<c_base_player*>(ent)->get_name() : ent->get_print_name();
	
	
	const auto text_size = ImGui::CalcTextSize(text.c_str());
	//const auto position = ImVec2(box.get_max().x - box.w / 2 - text_size.x / 2, box.get_min().y - text_size.y / 2);
	const auto position = ImVec2(box.x + (box.w * 0.5f - text_size.x * 0.5f), box.y - text_size.y * 1.1f);

	draw_list->AddTextOutlined(position, ent->get_team_color().get_u32(), c_color(0, 0, 0, 255).get_u32() , text.c_str());
	ImGui::PopFont();
}

inline void draw_box(c_base_entity* ent, math::c_box& box)
{
	const auto color = settings::visuals::esp_box_color_by_team
		                   ? ent->get_team_color().get_u32()
		                   : c_color(settings::colors::colors_map["esp_box_color"]).get_u32();

	const auto box_type = static_cast<int>(settings::visuals::esp_box_type);
	
	if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::flat))
	{
		draw_list->AddRect(box.get_min(), box.get_max(), color);
	}
	else if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::bounding))
	{
		draw_list->AddRect(ImVec2(box.x - 1.f, box.y - 1.f), ImVec2(box.x + box.w + 1.f, box.y + box.h + 1.f), c_color(0, 0, 0, 255).get_u32(), 0, ImDrawCornerFlags_All, 1);
		draw_list->AddRect(ImVec2(box.x + 1.f, box.y + 1.f), ImVec2(box.x + box.w - 1.f, box.y + box.h - 1.f), c_color(0, 0, 0, 255).get_u32(), 0, ImDrawCornerFlags_All, 1);

		draw_list->AddRect(ImVec2(box.x, box.y), ImVec2(box.x + box.w, box.y + box.h), color, 0, ImDrawCornerFlags_All, 2);
	}
	else if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::corners))
	{
		draw_list->AddRect(ImVec2(box.x, box.y), ImVec2(box.x + box.w, box.y + box.h), color, 10.f, ImDrawCornerFlags_All, 2);
	}
}

bool entity_in_vector(c_base_entity* ent)
{
	for (auto element : settings::visuals::entitys_to_draw)
	{
		if (element == ent->get_class_name())
			return true;
	}
	return false;
}

void visuals::esp::run_esp()
{
	if (!interfaces::engine->is_in_game() || !settings::visuals::esp)
		return;
	
	draw_list = ImGui::GetOverlayDrawList();
	
	for (auto i = 0; i <= interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* ent = get_entity_by_index(i);

		if (!ent)
			continue;
		
		const auto is_draw = entity_in_vector(ent) || ent->is_player();

		const auto has_owner = interfaces::entity_list->get_entity_by_handle(ent->get_owner_entity_handle()) ? true : false;
		
		if (!is_draw || !ent->is_alive() || ent == get_local_player() || has_owner)
			continue;
		
		math::c_box box{};
		if (!game_utils::get_entity_box(ent, box))
			continue;

		if (settings::visuals::esp_box)
			draw_box(ent, box);

		if (settings::visuals::esp_name)
			draw_name(ent, box);

		if (settings::visuals::esp_health)
			draw_health(ent, box);
	}
}