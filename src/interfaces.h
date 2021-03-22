#pragma once
#include <memory>

#include "game_sdk/interfaces/client.h"
#include "game_sdk/interfaces/engine.h"
#include "game_sdk/interfaces/surface.h"
#include "game_sdk/interfaces/i_input_system.h"
#include "game_sdk/interfaces/i_client_mode.h"
#include "game_sdk/interfaces/v_client_entity_list.h"
#include "game_sdk/interfaces/lua_shared.h"

namespace interfaces
{
	inline c_engine_client* engine;
	inline c_client* client;
	inline i_surface* surface;
	inline i_input_system* input_system;
	inline i_client_mode* client_mode;
	inline v_client_entity_list* entity_list;
	inline c_lua_shared* lua_shared;
	
	void init_interfaces();
}