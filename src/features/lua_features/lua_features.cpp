#include "lua_features.h"


#include <iostream>
#include <vector>
#include <mutex>

#include "../../interfaces.h"
#include "../../utils/game_utils.h"

std::mutex run_mutex;

std::vector<std::string> codes_to_run;
std::vector<std::string> codes_to_run_safe;

void lua_features::add_code_to_run(const std::string& str)
{
	codes_to_run.clear();
	codes_to_run.push_back(str);
	
	{
		std::unique_lock l(run_mutex);
		codes_to_run.swap(codes_to_run_safe);
	}
}

void lua_features::run_all_code()
{
	std::unique_lock l(run_mutex);
	auto lua = interfaces::lua_shared->get_interface((int)e_type::client);
	if (lua)
	{
		for (auto i : codes_to_run_safe)
		{
			try 
			{
				lua::push_hack_globals(lua), lua->run_string(last_name.c_str(), "", i.c_str(), true, true);
			}
			catch (...)
			{
			}
		}
	}

	codes_to_run_safe.clear();
}

std::string lua_code::lemi_code = u8R"(
local lemi = lemi_handler;
_G.lemi_handler = nil;

local lem = {};

lem.hooks = {};
lem.hooks_names = {};

local dbginf = debug.getinfo;
local isstr, isfunc = isstring, isfunction;

function lem.hookfn ( old, new, name )
	name = name or "";
	lem.hooks[new] = old;
	lem.hooks_names[new] = name;
	return new;
end

debug.getinfo = lem.hookfn(debug.getinfo, function(func, ...)
	local n_f = func;
	if lem.hooks[func] then n_f = lem.hooks[func] end;
	return dbginf(n_f, ... );
end, "debug.getinfo__");

local sfenv = debug.setfenv;
debug.setfenv = lem.hooks(debug.setfenv, function (obj, env)
	if lem.hooks[obj] then obj = function() end end
	return sfenv(obj, env);
end, "debug.setfenv__");

local dbggetlocal = debug.getlocal;
debug.getlocal = lem.hookfn( debug.getlocal, function( thread, level, index )
    local n_thread = thread;
    if isfunc(n_thread) && lem.hooks[n_thread] then n_thread = lem.hooks[n_thread] end;
    return dbggetlocal(n_thread or thread, level, index);
end, "debug.getlocal__");

local stump = string.dump;
string.dump = lem.hooks( string.dump, function( func, ... )
    local n_func = func;
    if lem.hooks[func] then n_func = lem.hooks[func] end;
    return stump(n_func, ... );
end,"string.dump___");

local dsmeta = debug.setmetatable;
local metatab = {};
debug.setmetatable = lem.hookfn( debug.setmetatable, function( tab, meta )
    if tab == lem.hooks then tab = metatab end;
    return dsmeta( tab, meta );
end, "debug.setmetatable__");

local uvj = debug.upvaluejoin;
debug.upvaluejoin = lem.hookfn( debug.upvaluejoin, function( f1, n1, f2, n2 )
    local n_func = f1;
    local n_func2 = f2;
    if lem.hooks[f1] then n_func = lem.hooks[f1] end;
    if lem.hooks[f2] then n_func2 = lem.hooks[f2] end;
    return uvj(n_func, n1, n_func2, n2);
end,"debug.upvaluejoin__");

)";