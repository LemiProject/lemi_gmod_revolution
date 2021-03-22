#pragma once
#include <stdint.h>

#include "../game_sdk/misc/recv_props.h"
#include "../game_sdk/misc/c_client_class.h"

#include <fnv.h>

#define NETVAR(table, prop, func_name, type) \
	type& func_name( ) { \
      static uintptr_t offset = 0; \
      if(!offset) \
      { offset = netvar_manager::get_net_var(fnv::hash( table ), fnv::hash( prop ) ); } \
	  \
      return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }

#define NETVAR_PTR(table, prop, func_name, type) \
	type* func_name( ) { \
      static uintptr_t offset = 0; \
      if(!offset) \
      { offset = netvar_manager::get_net_var(fnv::hash( table ), fnv::hash( prop ) ); } \
	  \
      return reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }

#define OFFSET(type, var, offset) \
	type& var() { \
		return *(type*)(uintptr_t(this) + offset); \
	} \

namespace netvar_manager {
    uintptr_t get_net_var(uint32_t table, uint32_t prop);
    void init_netvar_manager();
}