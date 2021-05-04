#pragma once
#include <d3d9.h>
#include <imgui/imgui.h>
#include <memory>
#include <atomic>

#include "../math/math.h"
#include <color.h>
#include <functional>

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

	namespace vars
	{
		inline std::atomic_bool is_screen_grab;
		inline D3DMATRIX view_matrix;
	}
	
	void init();
	void shutdown();

	void on_scene_end(uintptr_t ret_address);
	void on_present();
	
	IDirect3DDevice9* get_device();
}


namespace surface_render
{
	namespace fonts
	{
		inline uint64_t in_game_font = NULL;
	}
	void init_surface_render();

	void reload_fonts();
	
	void filled_rect(math::box_t box, c_color color);
	void bordered_rect(math::box_t box, c_color color);
	void line(math::vec2_t pos1, math::vec2_t pos2, c_color color);
	void text(math::vec2_t pos, uint64_t font, std::string_view text, c_color color, bool centered = true);
	void outlined_circle(math::vec2_t pos, int radius, c_color color, int segments = 120);

	void corner_box(math::box_t box, c_color color);

	uint64_t create_font(std::string_view win_path, int size, int flags);
}


namespace directx_render
{
	enum e_font_flags
	{
		font_none = 0,
		font_centered_x = (1 << 0),
		font_centered_y = (1 << 1),
		font_centered = font_centered_x | font_centered_y,
		font_drop_shadow = (1 << 2),
		font_outline = (1 << 3)
	};
	
	void init_directx_render();

	void render_surface(std::function<void()> draw_surface);
	
	void filled_rect(math::box_t box, c_color color, float round = 0.f);
	void bordered_rect(math::box_t box, c_color color, float round = 0.f);
	void line(const ImVec2& pos1, const ImVec2& pos2, c_color color, float t = 1.f);
	void text(ImFont* font, const std::string& text, const ImVec2& pos, float size, c_color color, int flags);

	void outlined_circle(const ImVec2& pos, int radius, c_color color, int segments = 120, int t = 1);
	void filled_circle(const ImVec2& pos, int radius, c_color color, int segments = 120, int t = 1);
	
	void corner_box(math::box_t box, c_color color);
}
