#include <iostream>

#include "visuals.h"

#include "../../utils/game_utils.h"

#include <imgui/im_tools.h>
#include <color.h>

#include <fmt/core.h>


#include "../../render_system/render_system.h"
#include "../../settings/settings.h"

#include "../../globals.h"

void visuals::overlay::run_overlay()
{
	auto local_player = get_local_player();
	
	if (settings::visuals::draw_fov && interfaces::engine->is_in_game() && local_player)
	{
		auto ply_fov = globals::view::last_view_setup.fov;
		auto fov = settings::aim::legit_bot_fov;
		fov = fov > 180 ? 180 : fov;

		auto sw = 1920;
		auto sh = 1080;
		interfaces::engine->get_screen_size(sw, sh);

		auto ratio = (float)sw / (float)sh;
		auto screen_fov = atanf((ratio) * (0.75f) * tan(math::deg2rad(ply_fov * 0.5f)));

		auto radius = tanf(math::deg2rad(fov)) / tanf(screen_fov) * (sw * 0.5f);

		directx_render::outlined_circle(ImVec2(sw / 2, sh / 2), radius, c_color(0, 0, 0));
	}
}
