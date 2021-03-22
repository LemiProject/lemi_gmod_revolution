#pragma once
#include <cstdint>

class v_client_entity_list
{
public:
	virtual void init() = 0;
	virtual void unused_1() = 0;
	virtual void unused_2() = 0;
	virtual void* get_entity_by_index(int index) = 0;
	virtual void* get_entity_by_handle(uintptr_t handle) = 0;
	virtual void unused_5() = 0;
	virtual int get_highest_entity_index() = 0;
	virtual void unused_7() = 0;
	virtual void unused_8() = 0;
};