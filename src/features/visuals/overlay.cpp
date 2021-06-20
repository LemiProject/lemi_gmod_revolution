#include <iostream>

#include "visuals.h"

#include "../../utils/game_utils.h"

#include <imgui/im_tools.h>
#include <color.h>

#include <fmt/core.h>


#include "../../render_system/render_system.h"
#include "../../settings/settings.h"

#include "../../globals.h"

#include "../menu/menu.h"

static int overlay_frame_count = 0;

void visuals::overlay::run_overlay()
{
	auto local_player = get_local_player();

	if (!menu::menu_is_open() && !interfaces::engine->is_in_game()) {
		c_color text_color = color_from_hsv((int)((float)overlay_frame_count * 0.02f) % 360, 1, 1);
		text(render_system::fonts::in_game_font, "LemiProject", { 0, 0 }, 30.f, text_color, directx_render::e_font_flags::font_outline);
	}

	
	if (settings::states["visuals::draw_fov"] && interfaces::engine->is_in_game() && local_player) {
		auto ply_fov = globals::view::last_view_setup.fov;
		auto fov = settings::values["aim_bot::aim_bot_fov"];
		fov = fov > 180 ? 180 : fov;

		auto sw = 1920;
		auto sh = 1080;
		interfaces::engine->get_screen_size(sw, sh);

		auto ratio = (float)sw / (float)sh;
		auto screen_fov = atanf((ratio) * (0.75f) * tan(math::deg2rad(ply_fov * 0.5f)));

		auto radius = tanf(math::deg2rad(fov)) / tanf(screen_fov) * (sw * 0.5f);

		directx_render::outlined_circle(ImVec2(sw / 2, sh / 2), radius, c_color(0, 0, 0));
	}

	if (settings::states["hvh::fake_lags"] && globals::local_player_states::fakelagpos.is_valid()) {
		c_vector screen_flp;
		if (game_utils::world_to_screen(globals::local_player_states::fakelagpos, screen_flp)) 
			directx_render::filled_circle({ screen_flp.x, screen_flp.y }, 15, colors::red_color);
	}

	overlay_frame_count++;
}
