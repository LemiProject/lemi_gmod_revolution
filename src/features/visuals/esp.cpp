#include <iostream>

#include "visuals.h"

#include "../../utils/game_utils.h"

#include <imgui/im_tools.h>
#include <color.h>

#include <fmt/core.h>


#include "../../render_system/render_system.h"
#include "../../settings/settings.h"


void draw_health(c_base_entity* ent, math::box_t box)
{
	if (ent->is_alive())
	{
		const auto health = std::clamp(ent->get_health(), 0, ent->get_max_health());
		const auto pos = ImVec2(box.x - 6.f, box.y);

		const math::box_t void_box{ pos.x, pos.y, 3, box.h };
		auto health_box{ void_box };

		health_box.h = health * void_box.h / 100;
		health_box.y = void_box.y + void_box.h - health_box.h;

		const auto health_color = c_color(settings::colors::colors_map["esp_health_color_hp"]);
		const auto void_color = c_color(settings::colors::colors_map["esp_health_color_void"]);

		surface_render::filled_rect(void_box, void_color);
		surface_render::filled_rect(health_box, health_color);
	}
}

inline void draw_name(c_base_entity* ent, math::box_t& box)
{
	const auto text = ent->is_player() ? static_cast<c_base_player*>(ent)->get_name() : ent->get_print_name();


	int w, h;
	interfaces::surface->get_text_size(surface_render::fonts::in_game_font, get_wc_t(text.c_str()), w,h);
	const math::vec2_t text_size = {(float)w, (float)h};
	
	const auto position = math::vec2_t{ box.x + box.w * 0.5f, box.y + box.h};
	
	const auto color = ent->is_player() ? static_cast<c_base_player*>(ent)->get_team_color() : c_color(settings::colors::colors_map["esp_health_color_hp"]);

	surface_render::text(position, surface_render::fonts::in_game_font, text, color);
}

inline void draw_box(c_base_entity* ent, math::box_t& box)
{
	c_color color;
	if (ent->is_player())
		color = settings::visuals::esp_box_color_by_team
		                   ? static_cast<c_base_player*>(ent)->get_team_color()
		                   : c_color(settings::colors::colors_map["esp_box_color"]);
	else
		color = c_color(settings::colors::colors_map["esp_box_color"]);
	
	const auto box_type = static_cast<int>(settings::visuals::esp_box_type);
	
	if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::flat))
	{
		surface_render::bordered_rect(box, color);
	}
	else if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::bounding))
	{
		//draw_list->AddRect(ImVec2(box.x - 1.f, box.y - 1.f), ImVec2(box.x + box.w + 1.f, box.y + box.h + 1.f), c_color(0, 0, 0, 255).get_u32(), 0, ImDrawCornerFlags_All, 1);
		//draw_list->AddRect(ImVec2(box.x + 1.f, box.y + 1.f), ImVec2(box.x + box.w - 1.f, box.y + box.h - 1.f), c_color(0, 0, 0, 255).get_u32(), 0, ImDrawCornerFlags_All, 1);

		surface_render::bordered_rect(math::create_box({ box.x - 1.f, box.y - 1.f }, { box.x + box.w + 1.f, box.y + box.h + 1.f }), colors::black_color);
		surface_render::bordered_rect(math::create_box({ box.x + 1.f, box.y + 1.f }, { box.x + box.w - 1.f, box.y + box.h - 1.f }), colors::black_color);

		surface_render::bordered_rect(math::create_box({ box.x, box.y }, { box.x + box.w, box.y + box.h }), color);
		//draw_list->AddRect(ImVec2(box.x, box.y), ImVec2(box.x + box.w, box.y + box.h), color, 0, ImDrawCornerFlags_All, 2);
	}
	else if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::corners))
	{
		surface_render::corner_box(box, color);
	}
}

void visuals::esp::run_esp()
{
	if (!interfaces::engine->is_in_game() || !settings::visuals::esp)
		return;
	
	for (auto i = 0; i <= interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* ent = get_entity_by_index(i);

		if (!ent || !ent->is_alive() || ent->is_dormant())
			continue;
		
		const auto is_draw = settings::visuals::entitys_to_draw.exist(ent->get_class_name()) || ent->is_player();

		const auto has_owner = interfaces::entity_list->get_entity_by_handle(ent->get_owner_entity_handle()) ? true : false;
		
		if (!is_draw || ent == get_local_player() || has_owner)
			continue;
		
		math::box_t box{};
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

