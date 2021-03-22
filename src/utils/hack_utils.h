#pragma once
#include "../interfaces.h"
#include "../render_system/render_system.h"
#include "../hooks_manager/hooks_manager.h"

namespace hack_utils
{
	static inline HMODULE g_dll;

	inline HWND get_allocated_console_hwnd()
	{
		CHAR old_title[1024];
		const auto* unique_title = "SUPERMEGATITLE";

		GetConsoleTitle(old_title, 1024);
		// place a unique title so that we can use FindWindow
		SetConsoleTitle(unique_title);
		// update window
		SleepEx(100, 0);
		const auto hwnd_console = FindWindow(nullptr, unique_title);
		SetConsoleTitle(old_title);

		return hwnd_console;
	}
	
	inline void shutdown_hack()
	{
		render_system::shutdown();
		hooks_manager::shutdown();

		FreeConsole();

		DestroyWindow(get_allocated_console_hwnd());
	}
}
