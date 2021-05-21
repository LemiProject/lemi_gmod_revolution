#include "menu.h"


int menu::lua_ui_api::lua_api_add_checkbox__Imp(c_lua_interface* lua)
{
	c_lua_auto_pop p(lua);
	std::string name = lua->get_string(1);
	if (name.empty())
	{
		lua->arg_error(1, "expected string");
		return 0;
	}


	
	return 0;
}