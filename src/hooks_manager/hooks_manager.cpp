#include "hooks_manager.h"
#include <minpp/c_min_hook.h>
#include <memory>
#include <fmt/core.h>

#include "../interfaces.h"

#include <d3d9.h>
#include "../game_sdk/misc/c_view_setup.h"

#include "../features/menu/menu.h"

#include "../render_system/render_system.h"
#include "../utils/hack_utils.h"
#include "../utils/game_utils.h"

#include "imgui/imgui_impl_dx9.h"

inline unsigned int get_virtual(void* _class, const unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

std::shared_ptr<min_hook_pp::c_min_hook> minpp = nullptr;


#define CREATE_HOOK(_class, index, detour, original) create_hook(reinterpret_cast<void*>(get_virtual(_class, index)), \
	detour, reinterpret_cast<void**>(&original));

struct end_scene_hook
{
	static constexpr uint32_t idx = 42;

	using fn = long(__stdcall*)(IDirect3DDevice9*);
	static inline fn original = nullptr;
	static long __stdcall hook(IDirect3DDevice9* device);
};

struct reset_hook
{
	static const unsigned int idx = 16;

	using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	static long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	static inline fn original;
};

struct lock_cursor_hook
{
	static constexpr uint32_t idx = 62;

	using fn = void(__thiscall*)(i_surface*);
	static inline fn original = nullptr;
	static void __stdcall hook();
};

struct override_view_hook
{
	static constexpr uint32_t idx = 16;

	using fn = void(__thiscall*)(i_client_mode*, c_view_setup&);
	static inline fn original = nullptr;
	static void __stdcall hook(c_view_setup& setup);
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
