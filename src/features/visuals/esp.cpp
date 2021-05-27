#include <iostream>

#include "visuals.h"

#include "../../utils/game_utils.h"

#include <imgui/im_tools.h>
#include <color.h>

#include <fmt/core.h>

#include "../../game_sdk/entitys/c_base_weapon.h"
#include "../../render_system/render_system.h"
#include "../../settings/settings.h"

ImVec2 last_text_pos = {-1.f, -1.f};
std::string last_text;

inline float calc_text_size(c_base_entity* ent, math::box_t box)
{
	float size = 16.f;
	if (box.h < 55)
	{
		size -= size * (box.h / 100);
		if (size < 13.f)
			size = 13.f;
	}
	return size;
}

void draw_armor(c_base_player* ply, math::box_t box)
{
	if (ply->is_alive())
	{
		const auto armor = std::clamp((int)ply->get_armor(), 0, 100);
		const auto pos = ImVec2(box.x, box.y + box.h + 2.f);

		const math::box_t void_box{pos.x, pos.y, box.w, 2.f};
		auto armor_box{ void_box };

		armor_box.w = armor * void_box.w / 100;
		//armor_box.x = void_box.x + void_box.w - armor_box.w;

		const auto armor_color = c_color(settings::colors::colors_map["esp_armor_color_hp"]);
		const auto void_color = c_color(settings::colors::colors_map["esp_armor_color_void"]);

		directx_render::filled_rect(void_box, void_color);
		directx_render::filled_rect(armor_box, armor_color);
	}
}

bool is_entity_var(const std::string& ent_var, const std::string& ply_var)
{
	return settings::states[ent_var] || (settings::states["visuals::esp_global"] && settings::states[ply_var]);
}

void draw_health(c_base_entity* ent, math::box_t box)
{
	if (ent->is_alive())
	{
		const auto health = std::clamp(ent->get_health(), 0, 100);
		const auto pos = ImVec2(box.x - 6.f, box.y);

		const math::box_t void_box{ pos.x, pos.y, 3, box.h };
		auto health_box{ void_box };

		health_box.h = health * void_box.h / 100;
		health_box.y = void_box.y + void_box.h - health_box.h;

		const auto health_color = c_color(settings::colors::colors_map["esp_health_color_hp"]);
		const auto void_color = c_color(settings::colors::colors_map["esp_health_color_void"]);

		directx_render::filled_rect(void_box, void_color);
		directx_render::filled_rect(health_box, health_color);

		if (!ent->is_player() && !(settings::states["visuals::esp_health_text_entity"] || (settings::states["visuals::esp_global"] && settings::states["visuals::esp_health_text_player"])))
			return;

		if (ent->is_player() && !settings::states["visuals::esp_health_text_player"])
			return;

		auto text = std::to_string(health);
		auto font_size = calc_text_size(ent, box);
		auto text_size = render_system::fonts::in_game_font->CalcTextSizeA(font_size, FLT_MAX, 0, text.c_str());
		auto text_pos = last_text_pos;
		
		float g = 255 * (health / 100.f);
		c_color text_color = {255 - g, g, 0};

		directx_render::text(render_system::fonts::in_game_font, text, text_pos, font_size, text_color, directx_render::font_outline);

		last_text_pos = text_pos;
		last_text = text;
	}
}

inline void draw_name(c_base_entity* ent, math::box_t& box)
{
	const auto text = ent->is_player() ? static_cast<c_base_player*>(ent)->get_name() : ent->get_print_name();

	auto font_size = calc_text_size(ent, box);
	auto ts = render_system::fonts::in_game_font->CalcTextSizeA(font_size, FLT_MAX, 0.f, text.c_str());
	const math::vec2_t text_size = {ts.x, ts.y};
	
	//const auto position = math::vec2_t{ box.x + box.w * 0.5f/* - (text_size.x / 2.f)*/, box.y + box.h + text_size.y / 2};
	const auto position = math::vec2_t{ box.x + box.w * 0.5f, box.y - text_size.y / 2 };
	
	const auto color = ent->is_player() ? static_cast<c_base_player*>(ent)->get_team_color() : c_color(settings::colors::colors_map["esp_health_color_hp"]);

	directx_render::text(render_system::fonts::in_game_font, text, position.get_im_vec2(), font_size, color, directx_render::font_centered | directx_render::font_outline);

	if (ent->is_player())
	{
		auto sid = static_cast<c_base_player*>(ent)->get_steam_id();
		if (!sid.empty())
		{
			if (std::find(settings::other::friends.begin(), settings::other::friends.end(), sid) != settings::other::friends.end())
			{
				constexpr auto str = "F";

				auto name_size = ts;
				ts = render_system::fonts::in_game_font->CalcTextSizeA(font_size, FLT_MAX, 0.f, str);
				auto pos = ImVec2(box.x + box.w + ts.y / 2.f, box.y);

				if (last_text_pos.x >= 0 && last_text_pos.y >= 0)
					pos.y = last_text_pos.y + render_system::fonts::in_game_font->CalcTextSizeA(font_size, FLT_MAX, 0.f, last_text.c_str()).y;
				
				directx_render::text(render_system::fonts::in_game_font, str, pos, font_size, colors::green_color, directx_render::font_outline);

				last_text_pos = pos;
				last_text = str;
			}
		}
	}

	if (ent->is_player() && settings::states["visuals::esp_active_weapon_player"])
	{
		auto ply = (c_base_player*)ent;
		auto weapon = get_primary_weapon(ply);

		if (weapon)
		{
			auto weapon_name = weapon->get_print_name();
			auto fs = calc_text_size(ent, box);
			auto ts = render_system::fonts::in_game_font->CalcTextSizeA(fs, FLT_MAX, 0.f, weapon_name.c_str());
			auto pos = ImVec2(box.x + box.w / 2.f, box.y + box.h + ts.y / 2.f);

			//for armor
			pos.y += 3.f;
			
			directx_render::text(render_system::fonts::in_game_font, weapon_name, pos, fs, c_color(settings::colors::colors_map["esp_weapon_name_color"]), directx_render::font_centered | directx_render::font_outline);
		}
	}
}

inline void draw_user_group(c_base_player* ply, math::box_t& box)
{
	auto str = ply->get_user_group();
	if (str.empty())
		return;

	auto font_size = calc_text_size(ply, box);
	auto text_size = render_system::fonts::in_game_font->CalcTextSizeA(font_size, FLT_MAX, 0.f, str.c_str());
	auto text_pos = ImVec2(box.x + box.w + text_size.y / 2.f, box.y);

	text_pos.y = last_text_pos.y + render_system::fonts::in_game_font->CalcTextSizeA(font_size, FLT_MAX, 0.f, last_text.c_str()).y;

	auto is_admin = str.find("admin") != std::string::npos || str.find("owner") != std::string::npos
		|| str.find("king") != std::string::npos || str.find("moder") != std::string::npos;

	c_color color = is_admin ? c_color(settings::colors::colors_map["esp_usergoup_text_admin"]) : c_color( settings::colors::colors_map["esp_usergoup_text_user"]);
	
	text(render_system::fonts::in_game_font, str, text_pos, font_size, color, directx_render::font_outline);

	last_text_pos = text_pos;
	last_text = str;
}

inline void draw_box(c_base_entity* ent, math::box_t& box)
{
	c_color color;
	int box_type;
		if (ent->is_player()) {
			color = settings::states["visuals::esp_color_by_team_player"]
				? static_cast<c_base_player*>(ent)->get_team_color()
				: c_color(settings::colors::colors_map["esp_box_color"]);

			box_type = static_cast<int>(settings::values["visuals::esp_box_type_player"]);
		}
		else {
			color = c_color(settings::colors::colors_map["esp_box_color"]);
			box_type = static_cast<int>(settings::values["visuals::esp_box_type_entity"]);
		}
	
	if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::flat))
	{
		directx_render::bordered_rect(box, color);
	}
	else if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::bounding))
	{
		directx_render::bordered_rect(math::create_box({ box.x - 1.f, box.y - 1.f }, { box.x + box.w + 1.f, box.y + box.h + 1.f }), colors::black_color);
		directx_render::bordered_rect(math::create_box({ box.x + 1.f, box.y + 1.f }, { box.x + box.w - 1.f, box.y + box.h - 1.f }), colors::black_color);

		directx_render::bordered_rect(math::create_box({ box.x, box.y }, { box.x + box.w, box.y + box.h }), color);
	}
	else if (box_type == static_cast<int>(settings::visuals::e_esp_box_type::corners))
	{
		directx_render::corner_box(box, color);
	}
}

void visuals::esp::run_esp()
{
	if (!interfaces::engine->is_in_game())
		return;
	
	for (auto i = 0; i <= interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* ent = get_entity_by_index(i);

		if (!ent || !ent->is_alive())
			continue;

		if (ent->is_player() && !settings::states["visuals::esp_player_dormant"] && ent->is_dormant())
			continue;
		if (!ent->is_player() && !is_entity_var("visuals::esp_entity_dormant", "visuals::esp_player_dormant") && ent->is_dormant())
			continue;
		
		const auto is_draw =  ent->is_player() || settings::visuals::entitys_to_draw.exist(ent->get_class_name());
		const auto has_owner = interfaces::entity_list->get_entity_by_handle(ent->get_owner_entity_handle()) ? true : false;
		
		if (!is_draw || ent == get_local_player() || has_owner)
			continue;

		math::box_t box{};
		if (ent->is_player())
		{
			if (!game_utils::get_player_box(ent, box))
				continue;
		}
		else
		{
			if (!game_utils::get_entity_box(ent, box))
				continue;
		}

		auto ts = render_system::fonts::in_game_font->CalcTextSizeA(calc_text_size(ent, box), FLT_MAX, 0.f, "TEST");
		last_text_pos = ImVec2(box.x + box.w / 2.f + ts.y, box.y);
		last_text = "";
		
		if (ent->is_player())
			if (settings::states["visuals::esp_enabled_player"]) {
				if (get_local_player()->get_eye_pos().distance_to(ent->get_eye_pos()) > settings::values["visuals::esp_distance_player"])
					continue;

				if (settings::states["visuals::esp_box_player"])
					draw_box(ent, box);

				if (settings::states["visuals::esp_health_player"])
					draw_health(ent, box);

				if (settings::states["visuals::esp_player_user_group"])
					draw_user_group((c_base_player*)ent, box);
				
				if (settings::states["visuals::esp_name_player"])
					draw_name(ent, box);

				if (settings::states["visuals::esp_armor_player"])
					draw_armor((c_base_player*)ent, box);
			}
		
		if (!ent->is_player())
			if (is_entity_var("visuals::esp_enabled_entity", "visuals::esp_enabled_player"))
			{
				if (settings::states["visuals::esp_global"])
				{
						if (get_local_player()->get_eye_pos().distance_to(ent->get_eye_pos()) > settings::values["visuals::esp_distance_player"])
							continue;
				}
				else
						if (get_local_player()->get_eye_pos().distance_to(ent->get_eye_pos()) > settings::values["visuals::esp_distance_entity"])
							continue;
				
				if (is_entity_var("visuals::esp_box_entity", "visuals::esp_box_player"))
					draw_box(ent, box);

				if (is_entity_var("visuals::esp_health_entity", "visuals::esp_health_player"))
					draw_health(ent, box);
				
				if (is_entity_var("visuals::esp_name_entity", "visuals::esp_name_player"))
					draw_name(ent, box);
			}
	}
}

