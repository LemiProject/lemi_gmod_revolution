#pragma once
#include "../../interfaces.h"

#include <map>

namespace menu
{
	void toggle_menu();
	bool menu_is_open();
	void set_open_state(bool state);
	
	void init();
	void draw();

	namespace lua_ui_api
	{
		inline std::map<std::string, std::string> widgets;
		
		LUA_FUNCTION(lua_api_add_checkbox);
		LUA_FUNCTION(lua_api_check_checkbox);
	}
}
