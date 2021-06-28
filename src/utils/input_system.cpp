#include "input_system.h"

#include <imgui/imgui.h>

#include <map>
#include <string>
#include <vector>

struct bool_bind_t {
	int key;
	bool* bind_var;
	input_system::bind_type bind_type;
	bool can_be_null;
};

std::map<std::string, bool_bind_t> key_binds;

auto find_bind(const std::string& name) {
	return std::find_if(key_binds.begin(), key_binds.end(), [name](const auto& it) {
		return it.first == name;
		});
}

bool bind_exist(const std::string& name) {
	return find_bind(name) != key_binds.end();
}

void input_system::initialize_input_system() {
	
}

void input_system::add_bind(int key, const std::string& name, bool* var, bind_type bt, bool can_be_null)
{
	if (var && !bind_exist(name))
		key_binds.emplace(name, bool_bind_t{ key, var, bt, can_be_null });
	else if (bind_exist(name))
		set_bind_key(name, key);
}

/*void input_system::add_bind(int key, const std::string& name, bool* var, bind_type bt) {
	if (var && !bind_exist(name))
		key_binds.emplace(name, bool_bind_t{ key, var, bt });
	else if (bind_exist(name))
		set_bind_key(name, key);
}*/

void input_system::set_bind_key(const std::string& name, int key) {
	if (bind_exist(name)) {
		find_bind(name)->second.key = key;
	}
}

void input_system::remove_bind(const std::string& name) {
	if (bind_exist(name)) {
		key_binds.erase(find_bind( name));
	}
}

bool input_system::is_key_pressed(int key, bool null_is_true)
{
	if (!null_is_true) {
		if (key == 0)
			return false;
		return ImGui::IsKeyDown(key);
	}
	if (key <= 0)
		return true;
	return ImGui::IsKeyDown(key);
}

bool input_system::is_key_just_pressed(int key, bool null_is_true)
{
	if (!null_is_true) {
		if (key == 0)
			return false;
		return ImGui::IsKeyPressed(key);
	}
	if (key <= 0)
		return true;
	return ImGui::IsKeyPressed(key);
}

void input_system::process_binds() {
	/*if (key_binds.empty())
		return;
	
	for (auto i : key_binds) {
		switch (i.second.bind_type) {
			case bind_type::pressed: {
				*i.second.bind_var = is_key_pressed(i.second.key);
				break;
			}
			case bind_type::just_pressed: {
				*i.second.bind_var = is_key_just_pressed(i.second.key);
				break;
			}
		}
	}*/
}
