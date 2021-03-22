#pragma once

#include "../misc/c_client_class.h"

class c_client	//TODO: COMPLETE THIS https://i.imgur.com/ut06Dyc.png
{
public:
	virtual int init(void* app_system_factory, void* physics_factory, void* globals) = 0;
	virtual void post_init() = 0;
	virtual void shutdown() = 0;
	virtual bool replay_init(void* reply_factory) = 0;
	virtual bool replay_post_init() = 0;
	virtual void level_init_pre_entity(const char* name) = 0;
	virtual void level_init_post_entity() = 0;
	virtual void level_shutdown() = 0;
	virtual c_client_class* get_client_class() = 0;
};