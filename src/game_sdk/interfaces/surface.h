#pragma once
#include "i_app_system.h"
#include "../../utils/memory_utils.h"

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

	void end_drawing()
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