#include "menu.h"

#include <iostream>
#include <imgui/imgui.h>

#include "windows/bgwindow.h"

#include "styles.h"
#include "windows/main_window.h"

#include "../../interfaces.h"

#include <Windows.h>

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

void menu::draw()
{
	if (is_open)
	{
		//interfaces::input_system->enable_input(true);

		if (!interfaces::surface->is_cursor_visible())
			ImGui::GetIO().MouseDrawCursor = true;
		
		bg_window::draw();
		main_window::draw();
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
	}
}
