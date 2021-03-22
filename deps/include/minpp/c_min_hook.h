#pragma once
#include <exception>

#define MHPP_ALL_HOOKS 0

namespace min_hook_pp
{
	typedef void* PVOID;
	typedef void* LPVOID;
	
	class c_min_hook
	{
	public:
		c_min_hook();
		~c_min_hook();
		c_min_hook(c_min_hook&) = delete;
		auto unhook(PVOID hook = MHPP_ALL_HOOKS) const -> void;
		auto create_hook(LPVOID target, PVOID detour, LPVOID* original) -> bool;
		void disable_hook(PVOID hook = MHPP_ALL_HOOKS);
		void enable_hook(PVOID hook = MHPP_ALL_HOOKS);
	};
}