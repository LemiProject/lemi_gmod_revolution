#include "menu.h"

#include <iostream>
#include <imgui/imgui.h>

#include "windows/bgwindow.h"

#include "styles.h"
#include "windows/main_window.h"

#include "../../interfaces.h"

#include <Windows.h>


#include "../../globals.h"
#include "../../settings/settings.h"
#include "windows/overlay.h"

bool is_open = false;

void menu::toggle_menu()
{
	is_open = !is_open;
}

bool menu::menu_is_open()
{
	return is_open;
}

void menu::set_open_state(const bool state)
{
	is_open = state;
}

void menu::init()
{
	imgui_styles::std_dark_theme();
}

#ifdef _DEBUG

void draw_debug()
{
	using namespace ImGui;

	if (!interfaces::engine->is_in_game())
		return;
	
	SetNextWindowSize({ 300.f, 300.f }, ImGuiCond_Once);
	Begin(u8"келх опндфейр деаюц)))))");

	auto to_land_delta = globals::local_player_states::land_position - globals::local_player_states::position;

	auto vel = globals::local_player_states::velocity.to_string_friendly();
	auto va = globals::local_player_states::viewangles.to_string_friendly();
	auto pos = globals::local_player_states::position.to_string_friendly();
	auto land = globals::local_player_states::land_position.to_string_friendly();
	
	TextWrapped("Ping: %fms\nVelocity: x:%f, y:%f, z:%f\nVelocity: %f\nViewAngles: x:%f, y:%f, z:%f\nPosition: x:%f, y:%f, z:%f\nLandPosition: x:%f, y:%f, z:%f\nToLandDistance: %f",
		globals::game_data::ping,
		vel.x, vel.y, vel.z,
		globals::local_player_states::velocity.length(),
		va.x, va.y, va.z,
		pos.x, pos.y, pos.z,
		land.x, land.y, land.z,
		to_land_delta.length());

	Hotkey("NFD##DF", &settings::debug_vars::no_fall_key);

	Checkbox("TP#SDASD", &settings::states["world::third_person"]);
	Hotkey("TP KEY##ASDSAD", &settings::binds["world::third_person_key"]);

	//Checkbox("FL##SDASD", &settings::other::fakelag);
	//SliderInt("FFS##sdasdsd", &settings::other::fakelagval, 0, 30);

	Checkbox("LegitAA##sdasd", &settings::other::legit_aa);
	
	End();
}

#endif

void menu::draw()
{
	if (is_open)
	{
		//interfaces::input_system->enable_input(true);

		if (!interfaces::surface->is_cursor_visible())
			ImGui::GetIO().MouseDrawCursor = true;

		//imgui_overlay::draw();
		
		bg_window::draw();
		main_window::draw();
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
	}
#ifdef _DEBUG
	draw_debug();
#endif
}
