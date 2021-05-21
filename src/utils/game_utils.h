#pragma once
#include "../math/math.h"
#include "../interfaces.h"

#include "../game_sdk/entitys/c_base_player.h"

#include "../render_system/render_system.h"
#include "../settings/settings.h"

namespace game_utils
{
	inline DWORD matrix_offset = 0x0;
	
	inline bool screen_transform(const c_vector& in, c_vector& out)	//We are rendering using imgui, so we need to normalize view_matrix
	{
		auto exception_filter = [](int code, PEXCEPTION_POINTERS ex)
		{
			return EXCEPTION_EXECUTE_HANDLER;
		};

		__try
		{
			if (!interfaces::engine->is_in_game())
				return false;

			const D3DMATRIX& world_matrix = interfaces::engine->get_world_to_screen_matrix();
			
			const auto w = world_matrix.m[3][0] * in.x + world_matrix.m[3][1] * in.y + world_matrix.m[3][2] * in.z + world_matrix.m[3][3];
			if (w < 0.001f)
			{
				out.x *= 100000;
				out.y *= 100000;
				return false;
			}

			out.x = world_matrix.m[0][0] * in.x + world_matrix.m[0][1] * in.y + world_matrix.m[0][2] * in.z + world_matrix.m[0][3];
			out.y = world_matrix.m[1][0] * in.x + world_matrix.m[1][1] * in.y + world_matrix.m[1][2] * in.z + world_matrix.m[1][3];
			out.z = 0.0f;

			out.x /= w;
			out.y /= w;

			return true;
		}
		__except (exception_filter(GetExceptionCode(), GetExceptionInformation()))
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}
	}

	inline bool world_to_screen(const c_vector& in, c_vector& out)
	{
		if (!screen_transform(in, out))
			return false;

		int w, h;
		interfaces::engine->get_screen_size(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}
	
	inline bool get_entity_box(c_base_entity* ent, math::box_t& box_in)
	{
		c_vector flb, brt, blb, frt, frb, brb, blt, flt;

		const auto& origin = ent->get_render_origin();
		const auto min = ent->get_collidable_ptr()->mins() + origin;
		const auto max = ent->get_collidable_ptr()->maxs() + origin;
		
		c_vector points[] = {
			c_vector(min.x, min.y, min.z),
			c_vector(min.x, max.y, min.z),
			c_vector(max.x, max.y, min.z),
			c_vector(max.x, min.y, min.z),
			c_vector(max.x, max.y, max.z),
			c_vector(min.x, max.y, max.z),
			c_vector(min.x, min.y, max.z),
			c_vector(max.x, min.y, max.z)
		};

		if (!world_to_screen(points[3], flb) || !world_to_screen(points[5], brt)
			|| !world_to_screen(points[0], blb) || !world_to_screen(points[4], frt)
			|| !world_to_screen(points[2], frb) || !world_to_screen(points[1], brb)
			|| !world_to_screen(points[6], blt) || !world_to_screen(points[7], flt))
			return false;

		c_vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		auto left = flb.x;
		auto top = flb.y;
		auto right = flb.x;
		auto bottom = flb.y;

		if (left < 0 || top < 0 || right < 0 || bottom < 0)
			return false;

		for (int i = 1; i < 8; i++) {
			if (left > arr[i].x)
				left = arr[i].x;
			if (bottom < arr[i].y)
				bottom = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (top > arr[i].y)
				top = arr[i].y;
		}

		box_in.x = left;
		box_in.y = top;
		box_in.w = right - left;
		box_in.h = bottom - top;

		return true;
	}

	inline float get_fov(const c_vector& from, const c_vector& to)
	{
		auto delta = to - from;
		delta.normalize();
		return sqrtf(powf(delta.x, 2.f) + powf(delta.y, 2.f));
	}

	inline c_vector calc_angle(const c_vector& from, const c_vector& to)
	{
		c_vector ang;
		const auto delta = from - to;
		const auto hyp = delta.length2d();

		ang.y = std::atanf(delta.y / delta.x) * 57.295779513082f;
		ang.x = std::atanf(-delta.z / hyp) * -57.295779513082f;

		if (delta.x >= 0.f)
			ang.y += 180.f;
		
		return ang;
	}
}


namespace lua
{
	constexpr auto hack_table_name = "L";
	
	__forceinline void hook_call(const std::string& name, e_type t = e_type::client)
	{
		auto lua = interfaces::lua_shared->get_interface((int)t);
		if (lua)
		{
			c_lua_auto_pop p(lua);
			lua->push_special((int)e_special::glob);
			lua->get_field(-1, "hook");
			lua->get_field(-1, "Call");
			lua->push_string(name.c_str());
			lua->call(1, 0);
		}
	}

	__forceinline void push_hack_globals(c_lua_interface* lua)
	{
		if (!settings::states["lua::hack_globals"])
			return;
		
		c_lua_auto_pop p(lua);
		
		directx_render::directx_lua_api::push_all(lua);
	}

	__forceinline void push_cfunction(c_lua_interface* i, const std::string& name, void* fn)
	{
		i->push_special((int)e_special::glob);
			i->push_c_function(fn);
			i->set_field(-2, name.c_str());
		i->pop();
	}
}