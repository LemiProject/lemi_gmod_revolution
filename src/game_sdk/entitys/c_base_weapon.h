#pragma once
#include "c_base_player.h"

class c_base_combat_weapon : public c_base_entity
{
public:
	NETVAR("DT_BaseCombatWeapon", "m_nSequence", get_sequence, int);
	NETVAR("DT_BaseCombatWeapon", "m_nForceBone", get_froce_bone, int);
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", get_clip1, int);
	NETVAR("DT_BaseCombatWeapon", "m_iClip2", get_clip2, int);
	NETVAR("DT_BaseCombatWeapon", "m_iPrimaryAmmoType", get_primary_ammo_type, int);
	NETVAR("DT_BaseCombatWeapon", "m_iSecondaryAmmoType", get_secoundary_ammo_type, int);
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", get_next_primary_attack, float);
	NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", get_next_secondary_attack, float);
	NETVAR("DT_BaseCombatWeapon", "m_nNextThinkTick", get_next_think_tick, int);
	NETVAR("DT_BaseCombatWeapon", "m_iState", get_state, int);
	NETVAR("DT_BaseCombatWeapon", "m_hOwner", get_owner_handle, uintptr_t);

	c_vector& get_bullet_spread()
	{
		using fn = c_vector & (__thiscall*)(void*);
		return (*(fn**)this)[326](this);
	}

	std::string get_weapon_base()
	{
		auto lua = interfaces::lua_shared->get_interface((int)e_special::glob);
		push_entity();
		lua->get_field(-1, "Base");
		if (!lua->is_type(-1, (int)e_lua_type::type_string))
		{
			lua->pop(2);
			return "";
		}
		std::string out = lua->get_string(-1);
		lua->pop(2);
		return out;
	}
};

__forceinline c_base_combat_weapon* get_primary_weapon(c_base_player* ply)
{
	return static_cast<c_base_combat_weapon*>(interfaces::entity_list->get_entity_by_handle(ply->active_weapon_handle()));
}