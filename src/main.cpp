#include <Windows.h>
#include "interfaces.h"
#include <iostream>
#include <thread>
#include "render_system/render_system.h"
#include "hooks_manager/hooks_manager.h"
#include "utils/hack_utils.h"

inline void load()
{
	using namespace std::chrono_literals;
	
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
#endif
	
	interfaces::init_interfaces();
	render_system::init();
	hooks_manager::init();
}

BOOL WINAPI DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID reversed)
{
	hack_utils::g_dll = dll_instance;
	DisableThreadLibraryCalls(dll_instance);
	if (reason == DLL_PROCESS_ATTACH)
		std::thread(load).detach();
	return TRUE;
}