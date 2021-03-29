#pragma once
#include "color.h"
#include "i_app_system.h"
#include "../../utils/memory_utils.h"

__forceinline const wchar_t* get_wc_t(const char* c)
{
	const auto c_size = strlen(c) + 1;
	const auto wc = new wchar_t[c_size];
	mbstowcs(wc, c, c_size);
	return wc;
}

enum e_font_draw_type
{
	font_draw_default = 0,
	font_draw_nonadditive,
	font_draw_additive,
	font_draw_type_count = 2,
};

enum e_font_flags
{
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

typedef unsigned long h_font;
typedef unsigned int vpanel;

struct int_rect
{
	int x0;
	int y0;
	int x1;
	int y1;
};


class i_surface
{
public:
	int get_texture_id(const char* file_name)
	{
		using fn = int(__thiscall*)(i_surface*, const char*);
		return (*(fn**)this)[28](this, file_name);
	}

	void get_texture_file(int id, char* file_name, int max_len)
	{
		using fn = void(__thiscall*)(i_surface*, int, char*, int);
		return (*(fn**)this)[29](this, id, file_name, max_len);
	}

	void get_texture_file(int id, char* file_name, int hw_filter, bool force_reload)
	{
		using fn = void(__thiscall*)(i_surface*, int, char*, int, bool);
		return (*(fn**)this)[29](this, id, file_name, hw_filter, force_reload);
	}

	void unlock_cursor()
	{
		using fn = void(__thiscall*)(i_surface*);
		return (*(fn**)this)[61](this);
	}

	void lock_cursor()
	{
		using fn = void(__thiscall*)(i_surface*);
		return (*(fn**)this)[62](this);
	}

	void set_cursor_always_visible(bool state)
	{
		using fn = void(__thiscall*)(i_surface*, bool);
		return (*(fn**)this)[52](this, state);
	}

	bool is_cursor_visible()
	{
		using fn = bool(__thiscall*)(i_surface*);
		return (*(fn**)this)[53](this);
	}

	void set_drawing_color(int r, int g, int b, int a = 255)
	{
		using original_fn = void(__thiscall*)(void*, int, int, int, int);
		return (*(original_fn**)this)[11](this, r, g, b, a);
	}

	void set_drawing_color(c_color color)
	{
		set_drawing_color(color.r, color.g, color.b, color.a);
	}

	void draw_filled_rect(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(void*, int, int, int, int);
		return (*(original_fn**)this)[12](this, x0, y0, x1, y1);
	}

	void draw_outline_rect(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(void*, int, int, int, int);
		return (*(original_fn**)this)[14](this, x0, y0, x1, y1);
	}

	void draw_line(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(void*, int, int, int, int);
		return (*(original_fn**)this)[15](this, x0, y0, x1, y1);
	}

	void set_text_font(unsigned long font)
	{
		using fn = void(__thiscall*)(void*, unsigned long);
		return (*(fn**)this)[17](this, font);
	}

	void set_text_color(c_color color)
	{
		set_text_color(color.r, color.g, color.b, color.a);
	}

	void set_text_color(int r, int g, int b, unsigned int a = 0)
	{
		using fn = void(__thiscall*)(void*, int, int, int, unsigned int);
		return (*(fn**)this)[19](this, r, g, b, a);
	}

	void set_text_pos(int x, int y)
	{
		using fn = void(__thiscall*)(void*, int, int);
		return (*(fn**)this)[20](this, x, y);
	}

	void draw_text(const wchar_t* text, int text_len)
	{
		using fn = void(__thiscall*)(void*, const wchar_t*, int, int);
		return (*(fn**)this)[22](this, text, text_len, 0);
	}

	unsigned long create_font()
	{
		using fn = unsigned long(__thiscall*)(void*);
		return (*(fn**)this)[66](this);
	}

	bool set_font_style(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		using fn = bool(__thiscall*)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
		return (*(fn**)this)[67](this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		using fn = void(__thiscall*)(void*, unsigned long, const wchar_t*, int&, int&);
		return (*(fn**)this)[76](this, font, text, wide, tall);
	}

	void draw_outlined_circle(int x, int y, int radius, int segments)	//100
	{
		using fn = void(__thiscall*)(void*, int, int, int, int);
		return (*(fn**)this)[100](this, x, y, radius, segments);
	}


	void draw_filled_rect_array(int_rect* rects, int rect_num)
	{
		using fn = void(__thiscall*)(void*, void*, int);
		static fn func;

		if (!func)
			func = (fn)(memory_utils::pattern_scanner("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 48"));

		func(this, rects, rect_num);
	}
	
	void start_drawing()
	{
		using start_drawing_fn = void(__thiscall*)(i_surface*);
		static start_drawing_fn ptr_start_drawing;
		if (!ptr_start_drawing)
			ptr_start_drawing = reinterpret_cast<start_drawing_fn>(memory_utils::pattern_scanner("vguimatsurface.dll",
					"55 8B EC 64 A1 ?? ?? ?? ?? 6A FF 68 ?? ?? ?? ?? 50 64 89 25 ?? ?? ?? ?? 83 EC 14")
			);
		ptr_start_drawing(this);
	}

	void finish_drawing()
	{
		using end_drawing_fn = void(__thiscall*)(i_surface*);
		static end_drawing_fn ptr_end_drawing;
		if (!ptr_end_drawing)
			ptr_end_drawing = reinterpret_cast<end_drawing_fn>(memory_utils::pattern_scanner("vguimatsurface.dll",
				"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 64 89 25 ?? ?? ?? ?? 51 56 6A 00")
				);
		ptr_end_drawing(this);
	}
};