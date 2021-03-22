#include "c_min_hook.h"

#include "minhook/minhook.h"


min_hook_pp::c_min_hook::c_min_hook()
{
	if (MH_Initialize() != MH_OK)
		throw std::exception("Failed to initialize minhook");
}

min_hook_pp::c_min_hook::~c_min_hook()
{
	unhook();
	MH_Uninitialize();
}

void min_hook_pp::c_min_hook::unhook(PVOID hook) const
{
	MH_RemoveHook(hook);
}

bool min_hook_pp::c_min_hook::create_hook(LPVOID target, PVOID detour, LPVOID* original)
{
	const auto res = MH_CreateHook(target, detour, original);
	return res == MH_OK;
}

void min_hook_pp::c_min_hook::disable_hook(PVOID hook)
{
	MH_DisableHook(hook);
}

void min_hook_pp::c_min_hook::enable_hook(PVOID hook)
{
	MH_EnableHook(hook);
}
