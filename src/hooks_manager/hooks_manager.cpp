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
#include "../utils/input_system.h"

#include "imgui/imgui_impl_dx9.h"

#include <intrin.h>
#include <iostream>

#include "../globals.h"
#include "../features/lua_features/lua_features.h"
#include "../features/menu/windows/bgwindow.h"
#include "../features/movement/movement.h"
#include "../features/prediction_system/prediction_system.h"
#include "../features/hvh/hvh.h"

#include "../settings/settings.h"

inline unsigned int get_virtual(void* _class, const unsigned int index) {
	return static_cast<unsigned int>((*static_cast<int**>(_class))[index]);
}

std::shared_ptr<min_hook_pp::c_min_hook> minpp = nullptr;

#define CREATE_HOOK(_class, index, detour, original) create_hook(reinterpret_cast<void*>(get_virtual(_class, index)), \
	detour, reinterpret_cast<void**>(&original));

struct end_scene_hook {
	static inline constexpr uint32_t idx = 42;

	using fn = long(__stdcall*)(IDirect3DDevice9*);
	static inline fn original = nullptr;
	static long __stdcall hook(IDirect3DDevice9* device);
};

struct present_hook //17
{
	static inline constexpr uint32_t idx = 17;

	using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	static inline fn original = nullptr;
	static long __stdcall hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override,
	                           RGNDATA* dirty_region);
};

struct reset_hook {
	static inline constexpr unsigned int idx = 16;

	using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	static long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	static inline fn original;
};

struct lock_cursor_hook {
	static inline constexpr uint32_t idx = 62;

	using fn = void(__thiscall*)(i_surface*);
	static inline fn original = nullptr;
	static void __stdcall hook();
};

struct override_view_hook {
	static inline constexpr uint32_t idx = 16;

	using fn = void(__thiscall*)(i_client_mode*, c_view_setup*);
	static inline fn original = nullptr;
	static void __stdcall hook(c_view_setup* setup);
};

struct create_move_hook {
	static inline constexpr uint32_t idx = 21;

	using fn = bool(__thiscall*)(i_client_mode*, float, c_user_cmd*);
	static inline fn original = nullptr;
	static bool __stdcall hook(float frame_time, c_user_cmd* cmd);
};

struct read_pixels_hook {
	static inline constexpr uint32_t idx = 11;

	using fn = void(__thiscall*)(i_mat_render_context*, int, int, int, int, unsigned char*, int);
	static inline fn original = nullptr;
	static void __fastcall hook(i_mat_render_context* self, uintptr_t edx, int x, int y, int w, int h, uint8_t* data,
	                            int dst);
};

struct view_render_hook {
	static inline constexpr uint32_t idx = 26;

	using fn = void(__thiscall*)(void*, void*);
	static inline fn original = nullptr;
	static void __fastcall hook(void* self, void* edx, void* rect);
};

struct render_view_hook {
	static inline constexpr uint32_t idx = 6;

	using fn = void(__thiscall*)(void*, c_view_setup&, int, int);
	static inline fn original = nullptr;
	static void __fastcall hook(i_view_render* view_render, void* edx, c_view_setup& setup, int clear_flags,
	                            int what_to_draw);
};

struct draw_model_execute_hook {
	static inline constexpr uint32_t idx = 20;

	using fn = void(__thiscall*)(iv_model_render*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*);
	static inline fn original = nullptr;
	static void __stdcall hook(draw_model_state_t& draw_state, model_render_info_t& render_info, matrix3x4_t* bone);
};

struct run_command_hook {
	static inline constexpr uint32_t idx = 17;

	using fn = void(__fastcall*)(i_prediction*, void*, c_base_entity*, c_user_cmd*, i_move_helper*);
	static inline fn original = nullptr;
	static void __fastcall hook(i_prediction* pred, void* edx, c_base_entity* player, c_user_cmd* ucmd,
	                            i_move_helper* move_helper);
};

struct paint_traverse_hook {
	static inline constexpr uint32_t idx = 41;

	using fn = void(__thiscall*)(i_panel*, uint32_t, bool, bool);
	static inline fn original = nullptr;
	static void __fastcall hook(i_panel* self, void*, unsigned int panel, bool force_repaint, bool allow_force);
};

struct run_string_ex //https://imgur.com/MLVf2V0
{
	static inline constexpr uint32_t idx = 111;

	using fn = bool(__thiscall*)(void*, const char*, const char*, const char*, bool, bool, bool, bool);
	static inline fn original = nullptr;
	static bool __fastcall hook(c_lua_interface* self, void* edx, const char* filename, const char* path,
	                            const char* string_to_run, bool run, bool print_errors, bool dont_push_errors,
	                            bool no_returns);
};

struct frame_stage_notify_hook {
	static inline constexpr uint32_t idx = 35;
	using fn = void(__thiscall*)(void*, int);
	static inline fn original = nullptr;
	static void __fastcall hook(c_client* client, void* edx, int stage);
};

struct wndproc_hook {
	static LRESULT STDMETHODCALLTYPE hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param);
	static inline WNDPROC original_wndproc = nullptr;

};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//void run_string_updater() //yeah, it`s best way hd)))
//{
//	auto lua = interfaces::lua_shared->get_interface(0);
//	while (!lua)
//		lua = interfaces::lua_shared->get_interface(0);
//	
//	hooks_manager::CREATE_HOOK(lua, run_string_ex::idx, run_string_ex::hook, run_string_ex::original);
//	minpp->enable_hook(reinterpret_cast<void*>(get_virtual(lua, run_string_ex::idx)));
//}

void hooks_manager::init() {
	minpp = std::make_shared<min_hook_pp::c_min_hook>();

	//uintptr_t present = (uintptr_t)memory_utils::pattern_scanner("gameoverlayrenderer.dll", "FF 15 ?? ?? ?? ?? 8B F8 85 DB") + 2;
	//uintptr_t reset = (uintptr_t)memory_utils::pattern_scanner("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 2;

	CREATE_HOOK(render_system::get_device(), end_scene_hook::idx, end_scene_hook::hook, end_scene_hook::original);
	CREATE_HOOK(render_system::get_device(), reset_hook::idx, reset_hook::hook, reset_hook::original);
	//CREATE_HOOK(render_system::get_device(), present_hook::idx, present_hook::hook, present_hook::original);
	//create_hook(**reinterpret_cast<void***>(present), (void*)present_hook::hook, reinterpret_cast<void**>(&present_hook::original));
	//create_hook(**reinterpret_cast<void***>(reset), reset_hook::hook, reinterpret_cast<void**>(&reset_hook::original));

	CREATE_HOOK(interfaces::surface, lock_cursor_hook::idx, lock_cursor_hook::hook, lock_cursor_hook::original);
	CREATE_HOOK(interfaces::client_mode, override_view_hook::idx, override_view_hook::hook,
	            override_view_hook::original);
	CREATE_HOOK(interfaces::client_mode, create_move_hook::idx, create_move_hook::hook, create_move_hook::original);
	CREATE_HOOK(interfaces::render_context, read_pixels_hook::idx, read_pixels_hook::hook, read_pixels_hook::original);
	CREATE_HOOK(interfaces::client, view_render_hook::idx, view_render_hook::hook, view_render_hook::original);
	CREATE_HOOK(interfaces::model_render, draw_model_execute_hook::idx, draw_model_execute_hook::hook,
	            draw_model_execute_hook::original);
	CREATE_HOOK(interfaces::prediction, run_command_hook::idx, run_command_hook::hook, run_command_hook::original);
	CREATE_HOOK(interfaces::panel, paint_traverse_hook::idx, paint_traverse_hook::hook, paint_traverse_hook::original);
	CREATE_HOOK(interfaces::view_render, render_view_hook::idx, render_view_hook::hook, render_view_hook::original);
	CREATE_HOOK(interfaces::client, frame_stage_notify_hook::idx, frame_stage_notify_hook::hook,
	            frame_stage_notify_hook::original);

	if (const auto run_string_ex_fn_ptr = reinterpret_cast<run_string_ex::fn>(memory_utils::pattern_scanner(
		"lua_shared.dll", "55 8B EC 8B 55 10 81 EC ? ? ? ?")); run_string_ex_fn_ptr)
		create_hook(run_string_ex_fn_ptr, run_string_ex::hook, reinterpret_cast<void**>(&run_string_ex::original));

	auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
	wndproc_hook::original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
		game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook::hooked_wndproc)));

	minpp->enable_hook();
}

void hooks_manager::shutdown() {
	minpp->unhook();
	minpp.reset();

	auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
	if (game_hwnd)
		SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook::original_wndproc));
}

void hooks_manager::create_hook(void* target, void* detour, void** original) {
	const auto result = minpp->create_hook(target, detour, original);
	if (!result)
		throw std::exception(fmt::format("Failed to create hook. Target: {}, detour: {}, original: {}",
		                                 reinterpret_cast<uintptr_t>(target), reinterpret_cast<uintptr_t>(detour),
		                                 reinterpret_cast<uintptr_t>(*original)).c_str());
}

void override_view_hook::hook(c_view_setup* setup) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return original(interfaces::client_mode, setup);

	if (game_utils::matrix_offset == 0x0)
		game_utils::matrix_offset = (reinterpret_cast<DWORD>(&interfaces::engine->get_world_to_screen_matrix()) + 0x40);

	if (!globals::local_player_states::real_fov)
		globals::local_player_states::real_fov = setup->fov;

	static bool fov_changed;
	if (settings::values["world::fov"])
		setup->fov = settings::values["world::fov"], fov_changed = true;
	else if (fov_changed)
		setup->fov = globals::local_player_states::real_fov, fov_changed = false;

	if (get_local_player() && get_local_player()->is_alive())
		if (settings::states["aim_bot::no_recoil"])
			setup->angles -= get_local_player()->get_view_punch_angles();

	if (!globals::local_player_states::in_third_person)
		if (globals::local_player_states::is_fake_duck)
			setup->origin.z = get_local_player()->get_origin().z + get_local_player()->get_view_offset(false).z;
			

	original(interfaces::client_mode, setup);
}

bool create_move_hook::hook(float frame_time, c_user_cmd* cmd) {
	auto fix_movement = [&](c_user_cmd& old_cmd) {
		c_base_player* local_player = get_local_player();

		if (!local_player || !local_player->is_alive())
			return;

		if (local_player->get_move_type() == (int)e_move_type::ladder)
			return;

		float f1, f2, yaw_delta = cmd->viewangles.y - old_cmd.viewangles.y;
		if (old_cmd.viewangles.y < 0.f)
			f1 = 360.0f + old_cmd.viewangles.y;
		else
			f1 = old_cmd.viewangles.y;

		if (cmd->viewangles.y < 0.0f)
			f2 = 360.0f + cmd->viewangles.y;
		else
			f2 = cmd->viewangles.y;

		if (f2 < f1)
			yaw_delta = abs(f2 - f1);
		else
			yaw_delta = 360.0f - abs(f1 - f2);

		yaw_delta = 360.0f - yaw_delta;

		cmd->forwardmove = cos(math::deg2rad(yaw_delta)) * old_cmd.forwardmove + cos(math::deg2rad(yaw_delta + 90.f)) *
			old_cmd.sidemove;
		cmd->sidemove = sin(math::deg2rad(yaw_delta)) * old_cmd.forwardmove + sin(math::deg2rad(yaw_delta + 90.f)) *
			old_cmd.sidemove;

		cmd->buttons &= ~IN_MOVERIGHT;
		cmd->buttons &= ~IN_MOVELEFT;
		cmd->buttons &= ~IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	};

	static auto choked_packets = 0;
	choked_packets++;

	DWORD move;
	_asm mov move, ebp;
	auto& send_packets = *(***reinterpret_cast<bool****>(move) - 1);

	game_utils::c_send_packets_helper send_packets_helper(send_packets);
	
	//update game data
	{
		if (get_local_player())
			globals::game_data::ping = get_local_player()->get_ping();
	}

	if (!cmd || cmd->command_number == 0 || !interfaces::engine->is_in_game()) {
		original(interfaces::client_mode, frame_time, cmd);
		return false;
	}

	auto* const lp = get_local_player();
	if (!lp || !lp->is_alive()) {
		original(interfaces::client_mode, frame_time, cmd);
		return false;
	}

	auto old_cmd = *cmd;
	globals::game_data::last_cmd = old_cmd;

	if (movement::free_cam::is_in_free_cam)
		cmd->buttons = 0, cmd->forwardmove = cmd->sidemove = cmd->upmove = 0.f, cmd->viewangles = old_cmd.viewangles;

	movement::run_movement(*cmd);

	engine_prediction.start(cmd, lp);
	{
		aim::run_aimbot(cmd);

		if (settings::states["aim_bot::aim_bot_trigger_bot"] && settings::get_bind_state(
			"aim_bot::aim_bot_trigger_bot_key")) {
			trace_t tr;
			game_utils::trace_view_angles(tr, cmd->viewangles);
			if (tr.m_pEnt) {
				if (reinterpret_cast<c_base_player*>(tr.m_pEnt)->is_player()) {
					auto ply = (c_base_player*)tr.m_pEnt;

					if (ply->is_alive() && game_utils::pass_aimbot_filters(ply))
						cmd->buttons |= IN_ATTACK;
				}
			}
		}

		aim::anti_recoil_and_spread(cmd);
	}
	engine_prediction.end();

	auto weapon = get_primary_weapon(lp);
	if (weapon)
		if (settings::states["other::rapid_fire"] && weapon->get_next_primary_attack() >= interfaces::global_vars->
			curtime)
			if (cmd->buttons & IN_ATTACK)
				cmd->buttons &= ~IN_ATTACK;

	if (movement::free_cam::is_in_free_cam)
		cmd->buttons = 0, cmd->forwardmove = cmd->sidemove = cmd->upmove = 0.f, cmd->viewangles = old_cmd.viewangles;

	original(interfaces::client_mode, frame_time, cmd);

	if (movement::free_cam::is_in_free_cam)
		cmd->buttons = 0, cmd->forwardmove = cmd->sidemove = cmd->upmove = 0.f, cmd->viewangles = old_cmd.viewangles;

	{
		static auto spawn_time = 0.f;
		static auto should_undo = false;
		if (settings::get_bind_state("exploits::auto_mega_jump", false)) {
			auto va = cmd->viewangles;
			auto vel = lp->get_velocity();
			if (vel.z < 0) {
				q_angle vela;
				math::vector_to_angel(vel, vela);
				vela.normalize();
				va = vela;

				trace_t tr;
				game_utils::trace_view_angles(tr, va);

#ifdef _DEBUG
				globals::local_player_states::land_position = tr.endpos;
#endif
				if ((tr.endpos - lp->get_origin()).length() <= 42.f && interfaces::global_vars->curtime > spawn_time +
					1.5f + (globals::game_data::ping / 100.f))
					interfaces::engine->execute_client_cmd("gm_spawn models/hunter/misc/shell2x2a.mdl"), spawn_time =
						interfaces::global_vars->curtime, should_undo = true;
			}
		}
		if (should_undo && interfaces::global_vars->curtime > spawn_time + 0.1f + (globals::game_data::ping / 100.f))
			interfaces::engine->execute_client_cmd("undo"), should_undo = false;
	}

	hvh::run_hvh(cmd);

	{
		static auto tick_counter = 0;
		static auto should_send = false;
		if (settings::states["hvh::fake_lags"] && !(cmd->buttons & IN_ATTACK) && !lp->is_speaking()) {
			if (tick_counter >= settings::values["hvh::fake_lags_value"])
				tick_counter = 0, should_send = !should_send;
			tick_counter++;
			if (should_send)
				send_packets_helper.set_force(true), globals::local_player_states::fakelagpos = lp->get_origin(), should_send = false, globals::local_player_states::is_fake_lag = false;
			else
				send_packets_helper.set_force(false), globals::local_player_states::is_fake_lag = true;
		}
		else {
			globals::local_player_states::fakelagpos = lp->get_origin();
		}
	}

	{
		static auto tick_counter = 0;
		static auto should_send = false;
		static q_angle old_ang;

		old_ang = cmd->viewangles;
		
		if (settings::other::legit_aa && !(cmd->buttons & IN_ATTACK) && !lp->is_speaking()) {
			if (settings::states["hvh::fake_lags"]) {
				if (!globals::local_player_states::is_fake_lag)
					cmd->viewangles.y += 90.f;
				else
					cmd->viewangles = old_ang;
			} else {
				if (tick_counter == 2)
					tick_counter = 0, should_send = !should_send;
				tick_counter++;
				if (should_send)
					send_packets_helper.set_force(true), cmd->viewangles = old_ang;
				else
					send_packets_helper.set_force(false), cmd->viewangles.y += 90.f;
			}
		}
	}
	
	{
		globals::local_player_states::is_fake_duck = false;

		static auto ticks_counter = 0;
		static auto should_send = false;
		if (settings::get_bind_state("hvh::fake_duck", false) && !(cmd->buttons & IN_ATTACK)) {
			globals::local_player_states::is_fake_duck = true;
			if (ticks_counter == 9)
				ticks_counter = 0, should_send = !should_send;
			ticks_counter++;
			if (should_send) {
				send_packets_helper.set_force(true, !settings::states["hvh::fake_lags"]);
				cmd->buttons |= IN_DUCK;
			}
			else {
				cmd->buttons &= ~IN_DUCK;
				send_packets_helper.set_force(false, !settings::states["hvh::fake_lags"]);
			}
		}
	}

	cmd->viewangles.clamp();
	cmd->viewangles.normalize();

	if (settings::states["misc::fix_movement"])
		fix_movement(old_cmd);

	{
		static auto spawn_time = 0.f;
		if (GetAsyncKeyState(settings::binds["exploits::wallpush"]) && settings::binds["exploits::wallpush"] != 0 &&
			interfaces::engine->get_last_time_stamp() > spawn_time + 1.f)
			interfaces::engine->execute_client_cmd("gm_spawn models/hunter/blocks/cube075x075x075.mdl ; sit ; undo"),
				spawn_time = interfaces::engine->get_last_time_stamp();
	}

	globals::local_player_states::viewangles = cmd->viewangles;
	globals::local_player_states::velocity = lp->get_velocity();
	globals::local_player_states::position = lp->get_origin();

	static auto cc = 0;
	cc++;
	if (settings::get_bind_state("other::add_entity", false) && cc >= 10) {
		cc = 0;
		trace_t tr;
		game_utils::trace_view_angles(tr, cmd->viewangles);

		if (tr.m_pEnt) {
			c_base_entity* ent = (c_base_entity*)tr.m_pEnt;
			if (ent->is_player()) {
				c_base_player* ply = (c_base_player*)ent;
				if (std::find(settings::other::friends.begin(), settings::other::friends.end(), ply->get_steam_id()) ==
					settings::other::friends.end())
					settings::other::friends.push_back(ply->get_steam_id());
				else
					settings::other::friends.erase(std::find(settings::other::friends.begin(),
					                                         settings::other::friends.end(), ply->get_steam_id()));
			}
			else {
				if (ent->get_class_name().find("worldspawn") == std::string::npos) {
					if (settings::visuals::entitys_to_draw.exist(ent->get_class_name()))
						settings::visuals::entitys_to_draw.remove(
							settings::visuals::entitys_to_draw.find(ent->get_class_name()));
					else
						settings::visuals::entitys_to_draw.push_back(ent->get_class_name());
				}
			}
		}
	}

	bg_window::update_entity_list();
	lua_features::run_all_code();
	//chams_manager::draw_chams();
	
	//if (GetAsyncKeyState(VK_MENU))
		//std::cout << lp->is_speaking() << std::endl;
	
	//if (globals::local_player_states::in_third_person)
	//get_local_player()->get_angles() = { 0, 0, 0 };
	
	send_packets_helper.apply(send_packets);
	
	if (settings::states["aim_bot::aim_bot_silent_aim"])
		return !settings::states["aim_bot::aim_bot_silent_aim"];

	return false;
}

void read_pixels_hook::hook(i_mat_render_context* self, uintptr_t edx, int x, int y, int w, int h, uint8_t* data,
                            int dst) {
	static uintptr_t render_capture;
	if (!render_capture)
		render_capture = reinterpret_cast<uintptr_t>(memory_utils::pattern_scanner("client.dll", "FF 50 ? 6A 20 8D 4D"))
			+ 3;

	c_view_setup setup;

	if (reinterpret_cast<uintptr_t>(_ReturnAddress()) == render_capture) {
		render_system::vars::_is_screen_grab = true;
		render_system::vars::last_sg_time = interfaces::global_vars->curtime;

		self->clear_buffers(true, true, true);

		//auto last_t = interfaces::input->camera_in_third_person;
		//if (last_t)
		//interfaces::input->camera_in_third_person = false;

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		auto get_player_view = [](c_view_setup& setup) {
			using fn = bool(__thiscall*)(void*, c_view_setup&);
			return (*reinterpret_cast<fn**>(interfaces::client))[59](interfaces::client, setup);
		};
		get_player_view(setup);

		auto render_view_client = [](const c_view_setup& setup, const int clear_flags, const int what_to_draw) {
			using fn = void(__thiscall*)(void*, const c_view_setup&, int, int);
			return (*reinterpret_cast<fn**>(interfaces::client))[27](interfaces::client, setup, clear_flags,
			                                                         what_to_draw);
		};
		render_view_client(globals::view::last_view_setup, (0x1 | 0x2), ((1 << 0) | (1 << 1)));

		render_system::vars::_is_screen_grab = false;

		//if (last_t)
		//interfaces::input->camera_in_third_person = true;
	}

	return original(self, x, y, w, h, data, dst);
}

void view_render_hook::hook(void* self, void* edx, void* rect) {
	original(self, rect);

	if (render_system::vars::_is_screen_grab)
		return;

	//chams_manager::draw_chams();
	
	interfaces::surface->start_drawing();
	{
		/*		if (settings::states["lua::hack_hooks"])
					lua::hook_call("LASGHudPaint");*/
	}
	interfaces::surface->finish_drawing();

	//interfaces::render_view.->Push3DView(globals->view, 0, nullptr, Interfaces->view_render->GetFrustum());
	
	
}

void render_view_hook::hook(i_view_render* view_render, void* edx, c_view_setup& setup, int clear_flags,
                            int what_to_draw) {
	//if (globals::local_player_states::viewangles.is_valid())
	//	setup.angles = globals::local_player_states::viewangles;

	globals::view::last_view_setup = setup;

	static auto prev_third_person = false;
	if (get_local_player() && !render_system::vars::is_screen_grab(interfaces::global_vars->curtime)) {
		static auto cc = 0;
		if (cc >= 30 && settings::get_bind_state("world::third_person_key", false))
			settings::states["world::third_person"] = !settings::states["world::third_person"], cc = 0;
		cc++;

		if (settings::states["world::third_person"]) {
			trace_t tr;
			ray_t ray;
			c_trace_filter f;
			f.pSkip = get_local_player();

			q_angle va;
			interfaces::engine->get_view_angles(va);

			c_vector ang_pos;
			math::angle_to_vector(va, ang_pos);
			ang_pos *= -1; //invert
			auto end_pos = setup.origin + (ang_pos * settings::values["world::third_person_distance"]);

			ray.init(setup.origin, end_pos);
			interfaces::engine_trace->trace_ray(ray, MASK_SOLID, &f, &tr);

			if (!(abs((tr.endpos - setup.origin).length()) < 50)) {
				interfaces::input->camera_in_third_person = true;
				prev_third_person = true;
				globals::local_player_states::in_third_person = true;

				setup.origin = tr.endpos;
				setup.angles = va;
			}
			else {
				if (prev_third_person)
					interfaces::input->camera_in_third_person = false, prev_third_person = false;
			}
		}
		else {
			if (prev_third_person)
				interfaces::input->camera_in_third_person = false, prev_third_person = false;
		}
	}

	static auto prev_free_cam = false;
	if (get_local_player() && !render_system::vars::is_screen_grab(interfaces::global_vars->curtime)) {
		static c_vector free_camera_position = {0.f};

		static auto cc = 0;
		if (cc >= 30 && settings::get_bind_state("world::free_camera_key", false))
			settings::states["world::free_camera"] = !settings::states["world::free_camera"], cc = 0;
		cc++;

		if (settings::states["world::free_camera"]) {
			interfaces::input->camera_in_third_person = true;
			movement::free_cam::run_free_cam(setup, free_camera_position);
			movement::free_cam::is_in_free_cam = true;
			prev_free_cam = true;
		}
		else {
			if (prev_free_cam)
				interfaces::engine->set_view_angles(globals::game_data::last_cmd.viewangles), interfaces::input->
					camera_in_third_person = false;
			prev_free_cam = false;
			free_camera_position = {0.f};
			movement::free_cam::is_in_free_cam = false;
		}

	}

	original(view_render, setup, clear_flags, what_to_draw);
}

//void __fastcall render_view_hook::hook(i_view_render* view_render, void* edx, c_view_setup& setup, int clear_flags,
//	int what_to_draw)
//{
//	globals::view::last_view_setup = setup;
//	
//	return original(view_render, setup, clear_flags, what_to_draw);
//}

void draw_model_execute_hook::hook(draw_model_state_t& draw_state, model_render_info_t& render_info,
                                   matrix3x4_t* bone) {
	auto is_draw = [](std::string_view ent) {
		if (settings::visuals::entitys_to_draw.empty() || !settings::states["visuals::entity_chams_enabled"])
			return false;
		return settings::visuals::entitys_to_draw.exist(ent.data());
	};

	if (chams_manager::is_chams_manager_calls())
		return original(interfaces::model_render, draw_state, render_info, bone);
	
	if (!settings::states["visuals::chams_enabled"] || !interfaces::engine->is_in_game() || render_system::vars::
		_is_screen_grab || interfaces::engine->is_taking_screenshot() || (settings::states["other::anti_obs"] &&
			settings::states["visuals::chams_obs_check"]))
		return original(interfaces::model_render, draw_state, render_info, bone);

	float color_modulation[4] = {1.f, 1.f, 0.f, 0.5f};

	i_material* material = interfaces::material_system->find_material(
		settings::strings["visuals::chams_material"].c_str(), "Model textures");
	material->add_ref();
	
	auto force_mat = [](bool ignore_z, float* color, i_material* mat) {
		interfaces::render_view->set_color_modulation(color);
		interfaces::render_view->set_blend(color[3]);
		mat->set_material_var_flag(material_var_ignorez, ignore_z);
		interfaces::model_render->forced_material_override(mat);
	};

	auto* ent = get_entity_by_index(render_info.entity_index);

	if (ent && material && ent->is_alive() && (ent->is_player() || is_draw(ent->get_class_name()))) {
		
		if (ent != get_local_player())
			force_mat(settings::states["visuals::ignore_z"],
			          settings::colors::colors_map["chams_color_modulation"].data(), material);
		else {
			//original(interfaces::model_render, draw_state, render_info, bone);
			
			/*if (settings::states["hvh::fake_lags"] && globals::local_player_states::fakelagpos.is_valid() && abs((globals::local_player_states::fakelagpos - ent->get_origin()).length()) >= 5.f)
				chams_manager::add_entity(ent, material, colors::black_color, globals::local_player_states::fakelagpos);*/

			
		}
	}

	chams_manager::draw_chams();
	
	original(interfaces::model_render, draw_state, render_info, bone);
	interfaces::model_render->forced_material_override(nullptr);
}

void run_command_hook::hook(i_prediction* pred, void* edx, c_base_entity* player, c_user_cmd* ucmd,
                            i_move_helper* move_helper) {
	if (settings::states["aim_bot::no_recoil"]) {
		q_angle angle;
		interfaces::engine->get_view_angles(angle);
		original(pred, edx, player, ucmd, move_helper);
		interfaces::engine->set_view_angles(angle);
	}
	else {
		original(pred, edx, player, ucmd, move_helper);
	}
}

void paint_traverse_hook::hook(i_panel* self, void* nn_var, unsigned panel, bool force_repaint, bool allow_force) {
	original(self, panel, force_repaint, allow_force);

	if (render_system::vars::_is_screen_grab)
		return;

	const std::string panel_name = interfaces::panel->get_name(panel);

	if (interfaces::engine->is_in_game()) {
		auto mat = interfaces::engine->get_world_to_view_matrix();
		render_system::vars::view_matrix = mat;
	}

	if (panel_name == "FocusOverlayPanel") {
		static auto numm = 0;
		numm++;

		directx_render::render_surface([&]() {
			visuals::run_visuals();
			if (settings::states["lua::hack_hooks"]) {
				lua::hook_call("LDrawVisuals");
				
			}
		});
	}

}

bool run_string_ex::hook(c_lua_interface* self, void* edx, const char* filename, const char* path,
                         const char* string_to_run, bool run, bool print_errors, bool dont_push_errors,
                         bool no_returns) {
	static c_lua_interface* last_self;
	static bool last_first;
	
	if (lua_features::is_hack_call) {
		return original(self, filename, path, string_to_run, run, print_errors, dont_push_errors, no_returns);
	}
	
	if (interfaces::engine->is_in_game() && settings::states["misc::runstring_block"] 
		&& !lua_features::is_hack_call && std::string(filename) != "RunString(Ex)") {
		if (get_local_player())
			get_local_player()->client_print(std::string("RunString Blocked! Filename: ") + filename, 3);
		return original(self, filename, path, "", run, print_errors, dont_push_errors, no_returns);
	}
	
	if (std::string(filename) == "RunString(Ex)") {
		last_self = self;
		return original(self, filename, path, string_to_run, run, print_errors, dont_push_errors, no_returns);
	}

	bool is_first = false;
	if (self != last_self && interfaces::engine->is_drawing_loading_image())
		is_first = true;

	last_self = self;

	if (is_first)
		std::cout << filename << std::endl;
		
	
	lua_features::last_name = filename;

	if (is_first) {
		lua_features::run_auto_run(self);
		
		std::string out_str = string_to_run;
		if (settings::other::load_bypass) {
			auto str_to_run = lua_code::lemi_code;
			str_to_run += u8"\n";
			str_to_run += string_to_run;
			out_str = str_to_run;
		}

		last_first = true;
		return original(self, filename, path, out_str.c_str(), run, print_errors, dont_push_errors, no_returns);
	}
	
	return original(self, filename, path, string_to_run, run, print_errors, dont_push_errors, no_returns);
}

void frame_stage_notify_hook::hook(c_client* client, void* edx, int stage) {
	enum e_frame_stage { frame_undefined = -1, frame_start = 0, frame_render_start = 5, frame_render_end = 6 };


	if (interfaces::input->camera_in_third_person && get_local_player() && get_local_player()->is_alive() && interfaces
		::engine->is_in_game())
		interfaces::prediction->set_local_view_angles(globals::local_player_states::viewangles);

	original(client, stage);
}

LRESULT STDMETHODCALLTYPE wndproc_hook::hooked_wndproc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param) {
	//input_system::process_binds();

	if (message_type == WM_CLOSE) {
		hack_utils::shutdown_hack();
		return true;
	}

	auto mk = VK_INSERT;
	if (settings::binds["other::menu_key"] > 0)
		mk = settings::binds["other::menu_key"];

	if (message_type == WM_KEYDOWN)
		if (w_param == mk)
			menu::toggle_menu();

	//ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param);
	if (ImGui_ImplWin32_WndProcHandler(window, message_type, w_param, l_param) && menu::menu_is_open()) {
		//interfaces::surface->unlock_cursor();
		return true;
	}

	return CallWindowProc(original_wndproc, window, message_type, w_param, l_param);
}

long end_scene_hook::hook(IDirect3DDevice9* device) {
	/*	static auto numm = 0;
		numm++;*/

	/*	if (settings::states["lua::hack_hooks"] && numm == 2)
		{
			lua::hook_call("LEndSceneHook");
			numm = 0;
		}*/

	auto ret = original(device);
	render_system::on_scene_end((uintptr_t)_ReturnAddress());
	return ret;
}

long present_hook::hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override,
                        RGNDATA* dirty_region) {
	//auto ret = original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
	//render_system::on_present();
	return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
}

long reset_hook::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	const auto ret = original(device, present_parameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();

	if (ret > 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

void lock_cursor_hook::hook() {
	if (menu::menu_is_open())
		interfaces::surface->unlock_cursor();
	else
		original(interfaces::surface);
}
