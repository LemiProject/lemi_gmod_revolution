#pragma once
#include <d3d9.h>
#include <imgui/imgui.h>
#include <memory>

namespace render_system
{
	namespace fonts
	{
		inline ImFont* main_font = nullptr;
		inline ImFont* title_font = nullptr;
		inline ImFont* in_game_font = nullptr;
	};

	namespace images
	{
		inline intptr_t* bg_logo_png;
	}
	
	void init();
	void shutdown();

	void on_scene_end();
	
	IDirect3DDevice9* get_device();
}
