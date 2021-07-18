#include "input_system.h"
#include <imgui/imgui.h>

#include <map>
#include <string>
#include <vector>
#include <array>

#include <mutex>

#include <Windows.h>

std::array<bool, 256> keys;
std::array<uint32_t, 256> frames;
uint32_t current_frame;

std::mutex input_mutex;

struct bool_bind_t {
	uint32_t* key;
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

void input_system::add_bind(uint32_t* key, const std::string& name, bool* var, bind_type bt, bool can_be_null)
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

void input_system::set_bind_key(const std::string& name, uint32_t* key) {
	if (bind_exist(name)) {
		find_bind(name)->second.key = key;
	}
}

void input_system::remove_bind(const std::string& name) {
	if (bind_exist(name)) {
		key_binds.erase(find_bind( name));
	}
}

bool input_system::is_key_pressed(int key, int delay, bool null_is_true)
{
	std::unique_lock l(input_mutex);
	
	if (key >= 256)
		return false;
	
	if (null_is_true)
		return keys[key] || key == 0;
	return keys[key];
}

bool input_system::is_key_just_pressed(int key, int delay, bool null_is_true)
{
	std::unique_lock l(input_mutex);
	
	if (key >= 256)
		return false;

	if (null_is_true)
		return (keys[key] && frames[key] + delay / 2 >= current_frame) || key == 0;
	return keys[key] && frames[key] + delay / 2 >= current_frame;
}

void input_system::process_binds() {
	if (key_binds.empty())
		return;
	
	for (auto& i : key_binds) {
		switch (i.second.bind_type) {
			case bind_type::pressed: {
				*i.second.bind_var = is_key_pressed(*i.second.key, 0);
				break;
			}
			case bind_type::just_pressed: {
				*i.second.bind_var = is_key_just_pressed(*i.second.key, 0);
				break;
			}
		}
	}
}

void input_system::on_windpoc(int msg, int wparam, int lparam)
{
	std::unique_lock l(input_mutex);
	
	current_frame++;
	
	if (msg == WM_SYSKEYDOWN || msg == WM_KEYDOWN) {
 			if (wparam < 256) {
				keys[wparam] = true;
				frames[wparam] = current_frame;
			}
	} else if (msg == WM_SYSKEYUP || msg == WM_KEYUP) {
		if (wparam < 256) {
			keys[wparam] = false;
			frames[wparam] = current_frame;
		}
	}

	if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) button = 1;
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) button = 2;
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) button = 3;
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) button = (HIWORD(wparam) == XBUTTON1) ? 5 : 4;
		keys[button] = true;
		frames[button] = current_frame;
	} else if (msg == WM_XBUTTONUP || msg == WM_LBUTTONUP || msg == WM_RBUTTONUP) {
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) button = 1;
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) button = 2;
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) button = 3;
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) button = (HIWORD(wparam) == XBUTTON1) ? 5 : 4;
		keys[button] = false;
		frames[button] = current_frame;
	}
}
