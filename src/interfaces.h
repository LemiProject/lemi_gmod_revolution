#pragma once
#include <memory>

#include "game_sdk/interfaces/client.h"
#include "game_sdk/interfaces/engine.h"
#include "game_sdk/interfaces/surface.h"
#include "game_sdk/interfaces/i_input_system.h"
#include "game_sdk/interfaces/i_client_mode.h"
#include "game_sdk/interfaces/v_client_entity_list.h"
#include "game_sdk/interfaces/lua_shared.h"
#include "game_sdk/interfaces/i_material_system.h"
#include "game_sdk/interfaces/iv_model_render.h"
#include "game_sdk/interfaces/i_render_view.h"

namespace interfaces
{
	inline c_engine_client* engine;
	inline c_client* client;
	inline i_surface* surface;
	inline i_input_system* input_system;
	inline i_client_mode* client_mode;
	inline v_client_entity_list* entity_list;
	inline c_lua_shared* lua_shared;
	inline i_material_system* material_system;
	inline i_mat_render_context* render_context;
	inline iv_model_render* model_render;
	inline iv_render_view* render_view;
	
	void init_interfaces();
}