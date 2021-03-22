#pragma once

namespace hooks_manager
{	
	void init();
	void shutdown();

	void create_hook(void* target, void* detour, void** original);
}
