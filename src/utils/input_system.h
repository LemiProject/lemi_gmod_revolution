#pragma once
#include <string>

//TODO: Optimization

namespace input_system {
	enum class bind_type : uint8_t {
		none = -1,
		pressed,
		just_pressed
	};
	
	void initialize_input_system();

	void add_bind(int key, const std::string& name, bool* var, bind_type bt = bind_type::pressed, bool can_be_null = false);
	void set_bind_key(const std::string& name, int key);
	void set_bind_state(const std::string& name, bind_type bt, bool can_be_null); // if bt == -1 then current value 
	void remove_bind(const std::string& name);
	
	bool is_key_pressed(int key, bool null_is_true = false);
	bool is_key_just_pressed(int key, bool null_is_true = false);

	void process_binds();
}