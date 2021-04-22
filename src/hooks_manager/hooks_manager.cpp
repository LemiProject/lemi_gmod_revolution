#include "hooks_manager.h"

#include <minpp/c_min_hook.h>
#include <memory>
#include <fmt/core.h>
#include <algorithm>

#include "../interfaces.h"

#include <d3d9.h>

#include "../game_sdk/misc/c_view_setup.h"

#include "../features/menu/menu.h"
#include "../features/visuals/visuals.h"

#include "../game_sdk/misc/c_user_cmd.h"
#include "../features/aim/aim.h"
#include "../game_sdk/entitys/c_base_weapon.h"
#include "../game_sdk/interfaces/i_material_system.h"

#include "../render_system/render_system.h"
#include "../utils/hack_utils.h"
#include "../utils/game_utils.h"

#include "imgui/imgui_impl_dx9.h"

#include <intrin.h>
#include <iostream>

#include "../settings/settings.h"

inline unsigned int get_virtual(void* _class, const unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

std::shared_ptr<min_hook_pp::c_min_hook> minpp = nullptr;


#define CREATE_HOOK(_class, index, detour, original) create_hook(reinterpret_cast<void*>(get_virtual(_class, index)), \
	detour, reinterpret_cast<void**>(&original));

struct end_scene_hook
{
	static inline constexpr uint32_t idx = 42;

	using fn = long(__stdcall*)(IDirect3DDevice9*);
	static inline fn original = nullptr;
	static long __stdcall hook(IDirect3DDevice9* device);
};

struct reset_hook
{
	static inline constexpr unsigned int idx = 16;

	using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	static long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	static inline fn original;
};

struct lock_cursor_hook
{
	static inline constexpr uint32_t idx = 62;

	using fn = void(__thiscall*)(i_surface*);
	static inline fn original = nullptr;
	static void __stdcall hook();
};

struct override_view_hook
{
	static inline constexpr uint32_t idx = 16;

	using fn = void(__thiscall*)(i_client_mode*, c_view_setup&);
	static inline fn original = nullptr;
	static void __stdcall hook(c_view_setup& setup);
};

struct create_move_hook
{
	static inline constexpr uint32_t idx = 21;

	using fn = bool(__thiscall*)(i_client_mode*, float, c_user_cmd*);
	static inline fn original = nullptr;
	static bool __stdcall hook(float frame_time, c_user_cmd* cmd);
};

struct read_pixels_hook
{
	static inline constexpr uint32_t idx = 11;

	using fn = void(__thiscall*)(i_mat_render_context*, int, int, int, int, unsigned char*, int);
	static inline fn original = nullptr;
	static void __fastcall hook(i_mat_render_context* self, uintptr_t edx, int x, int y, int w, int h, uint8_t* data, int dst);
};

struct view_render_hook
{
	static inline constexpr uint32_t idx = 26;

	using fn = void(__thiscall*)(void*, void*);
	static inline fn original = nullptr;
	static void __fastcall hook(void* self, void* edx, void* rect);
};

struct draw_model_execute_hook
{
	static inline constexpr uint32_t idx = 20;
	
	using fn = void(__thiscall*)(iv_model_render*, const draw_model_state_t&, const model_render_info_t&, void*);
	static inline fn original = nullptr;
	static void __stdcall hook(draw_model_state_t& draw_state, model_render_info_t& render_info, void* bone);
};


struct wndproc_hook
{
	static LRESULT STDMETHODCALLTYPE hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param);
	static inline WNDPROC original_wndproc = nullptr;
	
}; extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void hooks_manager::init()
{
	minpp = std::make_shared<min_hook_pp::c_min_hook>();

	CREATE_HOOK(render_system::get_device(), end_scene_hook::idx, end_scene_hook::hook, end_scene_hook::original);
	CREATE_HOOK(render_system::get_device(), reset_hook::idx, reset_hook::hook, reset_hook::original);
	CREATE_HOOK(interfaces::surface, lock_cursor_hook::idx, lock_cursor_hook::hook, lock_cursor_hook::original);
	CREATE_HOOK(interfaces::client_mode, override_view_hook::idx, override_view_hook::hook, override_view_hook::original);
	CREATE_HOOK(interfaces::client_mode, create_move_hook::idx, create_move_hook::hook, create_move_hook::original);
	CREATE_HOOK(interfaces::render_context, read_pixels_hook::idx, read_pixels_hook::hook, read_pixels_hook::original);
	CREATE_HOOK(interfaces::client, view_render_hook::idx, view_render_hook::hook, view_render_hook::original);
	CREATE_HOOK(interfaces::model_render, draw_model_execute_hook::idx, draw_model_execute_hook::hook, draw_model_execute_hook::original);
	
	auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
	wndproc_hook::original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
		game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook::hooked_wndproc)));


	
	minpp->enable_hook();
}

void hooks_manager::shutdown()
{
	minpp->unhook();
	minpp.reset();

	auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
	if (game_hwnd)
		SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook::original_wndproc));
}

void hooks_manager::create_hook(void* target, void* detour, void** original)
{
	const auto result = minpp->create_hook(target, detour, original);
	if (!result)
		throw std::exception(fmt::format("Failed to create hook. Target: {}, detour: {}, original: {}",
			reinterpret_cast<uintptr_t>(target), reinterpret_cast<uintptr_t>(detour),
			reinterpret_cast<uintptr_t>(*original)).c_str());
}

void override_view_hook::hook(c_view_setup& setup)
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return original(interfaces::client_mode, setup);

	if (game_utils::matrix_offset == 0x0)
		game_utils::matrix_offset = (reinterpret_cast<DWORD>(&interfaces::engine->get_world_to_screen_matrix()) + 0x40);
	
	original(interfaces::client_mode, setup);
}

bool create_move_hook::hook(float frame_time, c_user_cmd* cmd)
{
	static auto choked_packets = 0;
	choked_packets++;

	DWORD move;
	_asm mov move, ebp;
	auto& send_packets = *(***reinterpret_cast<bool****>(move)-1);

	auto ret = original(interfaces::client_mode, frame_time, cmd);

	if (!cmd)
		return ret;

	auto* const lp = get_local_player();
	if (!lp || !lp->is_alive())
		return ret;

	const auto old_cmd = *cmd;

	auto* weapon = get_primary_weapon(lp);
	if (weapon && cmd->buttons & IN_ATTACK)
	{
		auto& spread = weapon->get_bullet_spread();
		cmd->viewangles += spread;
	}
	
	aim::run_aimbot(cmd);
	
	cmd->viewangles.normalize();
	cmd->viewangles.clamp();
	
	return ret;
}

void read_pixels_hook::hook(i_mat_render_context* self, uintptr_t edx, int x, int y, int w, int h, uint8_t* data,
	int dst)
{
	static uintptr_t render_capture;
	if (!render_capture)
		render_capture = reinterpret_cast<uintptr_t>(memory_utils::pattern_scanner("client.dll", "FF 50 ? 6A 20 8D 4D")) + 3;

	c_view_setup setup;
	
	if (reinterpret_cast<uintptr_t>(_ReturnAddress()) == render_capture)
	{
		render_system::vars::is_screen_grab = true;

		self->clear_buffers(true, true, true);
		
		auto get_player_view = [](c_view_setup& setup)
		{
			using fn = bool(__thiscall*)(void*, c_view_setup&);
			return (*reinterpret_cast<fn**>(interfaces::client))[59](interfaces::client, setup);
		};
		get_player_view(setup);
		
		auto render_view_client = [](const c_view_setup& setup, const int clear_flags, const int what_to_draw)
		{
			using fn = void(__thiscall*)(void*, const c_view_setup&, int, int);
			return (*reinterpret_cast<fn**>(interfaces::client))[27](interfaces::client, setup, clear_flags, what_to_draw);
		};
		render_view_client(setup, (0x1 | 0x2), ((1 << 0) | (1 << 1)));
		
		render_system::vars::is_screen_grab = false;
	}

	return original(self, x, y, w, h, data, dst);
}

void view_render_hook::hook(void* self, void* edx, void* rect)
{
	original(self, rect);

	if (render_system::vars::is_screen_grab)
		return;
	
	interfaces::surface->start_drawing();

	visuals::run_visuals();
		
	interfaces::surface->finish_drawing();
}

void draw_model_execute_hook::hook(draw_model_state_t& draw_state, model_render_info_t& render_info, void* bone)
{
	auto is_draw = [](std::string_view ent)
	{
		if (settings::visuals::entitys_to_draw.empty() || !settings::visuals::entity_chams)
			return false;
		return settings::visuals::entitys_to_draw.exist(ent.data());
	};
	
	if (!settings::visuals::chams || !interfaces::engine->is_in_game() || render_system::vars::is_screen_grab || interfaces::engine->is_taking_screenshot())
		return original(interfaces::model_render, draw_state, render_info, bone);

	float color_modulation[4] = {1.f, 1.f, 0.f, 0.5f};

	i_material* material = interfaces::material_system->find_material(settings::visuals::chams_material.c_str(), "Model textures");
	material->increment_reference_count();
	
	auto force_mat = [](bool ignore_z, float* color, i_material* mat)
	{
		interfaces::render_view->set_color_modulation(color);
		interfaces::render_view->set_blend(color[3]);
		mat->set_material_var_flag(material_var_ignorez, ignore_z);
		interfaces::model_render->forced_material_override(mat);
	};

	auto* ent = get_entity_by_index(render_info.entity_index);

	if (ent && material && ent->is_alive() && (ent->is_player() || is_draw(ent->get_print_name())) && ent != get_local_player())
	{
		force_mat(settings::visuals::ignore_z, settings::colors::colors_map["chams_color_modulation"].data(), material);
	}

	original(interfaces::model_render, draw_state, render_info, bone);
	interfaces::model_render->forced_material_override(nullptr);
}

LRESULT STDMETHODCALLTYPE wndproc_hook::hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param)
{
	if (message_type == WM_CLOSE)
	{
		hack_utils::shutdown_hack();
		return true;
	}

	if (message_type == WM_KEYDOWN)
		if (w_param == VK_INSERT)
			menu::toggle_menu();

	//ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param);
	if (ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param) && menu::menu_is_open())
	{
		//interfaces::surface->unlock_cursor();
		return true;
	}

	return CallWindowProc(original_wndproc, window, message_type, w_param, l_param);
}

long end_scene_hook::hook(IDirect3DDevice9* device)
{
	render_system::on_scene_end();
	return original(device);
}

long reset_hook::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	const auto ret = original(device, present_parameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();

	if (ret > 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

void lock_cursor_hook::hook()
{
	if (menu::menu_is_open())
		interfaces::surface->unlock_cursor();
	else
		original(interfaces::surface);
}
