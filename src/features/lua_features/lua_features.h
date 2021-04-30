#pragma once

#include <string>

namespace lua_features
{
	void add_code_to_run(const std::string& str);

	void run_all_code();
}
