#include <iostream>

#include "aim.h"

#include "../../game_sdk/entitys/c_base_player.h"
#include "../../game_sdk/entitys/c_base_weapon.h"

#include "../../utils/game_utils.h"

#include "../../settings/settings.h"

#include "spreads/all_spreads.h"
#include "spreads/csbase.h"
#include "spreads/fas2nospread.h"
#include "spreads/weapon_base.h"

struct target_t
{
	c_base_player* ply;
	float fov;
	c_vector angle;
	int idx;
};

int last_target_id = -1;
float last_target_time = -1.f;

bool get_target_bone(c_base_player* ply, c_vector& out)
{
	using plb = settings::aimbot::e_player_bones;
	using namespace settings::aimbot;

	const auto flags = settings::flags["aim_bot::aim_bot_player_bones"];

	std::vector<std::string> bones;

	for (auto i = 1; i <= static_cast<int>(plb::last); ++i)
		if (flags & i && !bones_hl_names[i].empty())
			bones.push_back(bones_hl_names[i]);

	if (bones.empty())
		return false;
	
	c_vector real_ang;
	interfaces::engine->get_view_angles(real_ang);

	auto best_fov = FLT_MAX;
	std::string best_bone;
	for (auto i : bones)
	{
		if (!ply->get_bone(ply->get_bone_by_name(i)).is_valid())
			continue;
		const auto fov = game_utils::get_fov(real_ang, game_utils::calc_angle(get_local_player()->get_eye_pos(), ply->get_bone(ply->get_bone_by_name(i))));
		if (fov < best_fov)
			best_fov = fov, best_bone = i;
	}

	if (best_bone.empty())
		return false;
	
	out = ply->get_bone(ply->get_bone_by_name(best_bone));
	
	return true;
}

bool get_target(target_t& target)
{
	auto invalidate_target = [](const target_t& t)
	{
		return (t.ply != nullptr && t.angle.x != 0 && t.angle.y != 0); //fov maybe FLT_MAX and 0
	};
	
	if (!get_local_player())
		return false;

	target_t tmp{nullptr, FLT_MAX, {}};
	
	for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* player = get_player_by_index(i);

		if (!player || !player->is_player() || !player->is_alive() || player == get_local_player())
			continue;

		if (!game_utils::pass_aimbot_filters(player))
			continue;
		
		c_vector engine_angels;
		interfaces::engine->get_view_angles(engine_angels);
		c_vector hit_pos;
		if (!get_target_bone(player, hit_pos))
			continue;
		const auto fov = game_utils::get_fov(engine_angels,
		                                     game_utils::calc_angle(get_local_player()->get_eye_pos(),
												 hit_pos));
		
		if (fov < tmp.fov && fov <= settings::values["aim_bot::aim_bot_fov"] && player->is_visible_by(get_local_player()))
		{
			if (std::find(settings::other::friends.begin(), settings::other::friends.end(),player->get_steam_id()) != settings::other::friends.end())
				continue;
			if (std::find(settings::other::friendly_teams.begin(), settings::other::friendly_teams.end(), player->get_team_num()) != settings::other::friendly_teams.end())
				continue;
			
			tmp.fov = fov;
			tmp.ply = player;
			tmp.angle = math::get_angle(get_local_player()->get_eye_pos(), hit_pos);
			tmp.idx = i;
		}
	}

	if (!invalidate_target(tmp))
		return false;
		
	target = tmp;
	return true;
}

q_angle do_smooth(const q_angle& from, const q_angle& to, float smooth_val)
{
	auto delta = to - from;
	delta.normalize();
	delta /= q_angle(smooth_val, smooth_val, 0);
	auto out = to - delta;
	out.normalize();
	return out;
}

void aim::aim_bot(c_user_cmd* cmd)
{
	if (!settings::states["aim_bot::aim_bot_enabled"] || !interfaces::engine->is_in_game())
		return;

	if (settings::binds["aim_bot::aim_bot_key"] != 0)
		if (!GetAsyncKeyState(settings::binds["aim_bot::aim_bot_key"]))
			return;

	if (cmd->command_number == 0)
		return;
	
	auto* const lp = get_local_player();

	if (!cmd|| !lp || !lp->is_alive() || !lp->is_alive())
		return;

	auto* weapon = get_primary_weapon(lp);
	if (!weapon || !weapon->can_shoot())
		return;

	target_t target;

	auto is_auto_fire = settings::states["aim_bot::aim_bot_auto_fire"] && (GetAsyncKeyState(
			settings::binds["aim_bot::aim_bot_auto_fire_key"]) || settings::binds[
			"aim_bot::aim_bot_auto_fire_key"]
		== 0);
	
	if ((  (!(cmd->buttons & IN_ATTACK)) && !is_auto_fire) 
		|| weapon->get_next_primary_attack() <= interfaces::global_vars->interval_per_tick)
		return;
	
	if (!get_target(target))
		return;

	if (settings::values["aim_bot::aim_bot_delay_before_aiming"] > 0)
		if (last_target_id != target.idx && interfaces::engine->get_time_stamp_from_start() <= last_target_time +
			(settings::values["aim_bot::aim_bot_delay_before_aiming"] / 100.f) && !get_entity_by_index(last_target_id)->is_alive())
			return;
	
	if (settings::values["aim_bot::aim_bot_smooth_value"] > 0.f)
		target.angle = do_smooth(target.angle, cmd->viewangles, settings::values["aim_bot::aim_bot_smooth_value"]);

	last_target_id = target.idx;
	last_target_time = interfaces::engine->get_time_stamp_from_start();
	
	cmd->viewangles = target.angle;

	if (!settings::states["aim_bot::aim_bot_silent_aim"])
		interfaces::engine->set_view_angles(cmd->viewangles);

	if (is_auto_fire)
		cmd->buttons |= IN_ATTACK;
}

std::map<std::string, std::map<std::string, float>> recoil_for_weapons;

void aim::anti_recoil_and_spread(c_user_cmd* ucmd)
{
	auto* local_player = get_local_player();
	if (!local_player || !local_player->is_alive())
		return;

	auto* weapon = get_primary_weapon(local_player);
	if (!weapon)
		return;

	if (ucmd->command_number == 0)
		return;
	
	//if (recoil_for_weapons.find(weapon->get_weapon_base()) == recoil_for_weapons.end())
	//{
	//	std::map<std::string, float> tmp;
	//	weapon->get_recoil(tmp);
	//	recoil_for_weapons.emplace(weapon->get_weapon_base(), tmp);
	//}
	
	//if (settings::states["aim_bot::no_recoil"])
	//	ucmd->viewangles -= local_player->get_view_punch_angles();
	//else
	//	weapon->set_recoil(recoil_for_weapons[weapon->get_weapon_base()]);

	if (settings::states["aim_bot::no_recoil"])
		ucmd->viewangles -= local_player->get_view_punch_angles();
	
	if (settings::states["aim_bot::no_spread"] && ucmd->buttons & IN_ATTACK && weapon->can_shoot())
	{
		if (weapon->get_weapon_base().find("weapon_tttbase") != std::string::npos
			|| weapon->get_weapon_base().find("bobs_gun_base") != std::string::npos
			|| weapon->get_weapon_base().empty())
			calc_spread_csbase(weapon, ucmd);
		else if (weapon->get_weapon_base().find("swb") != std::string::npos
			|| weapon->get_weapon_base().find("cw_") != std::string::npos)
			allspreads_nospread(weapon, ucmd);
		else if (weapon->get_class_name().find("fas2") != std::string::npos)
			fas2nospread(weapon, ucmd);
			
	}
}


//const auto spread_cone = weapon->get_spread();
//const auto spread = -((spread_cone.x + spread_cone.y + spread_cone.z) / 3.f);
//
//float random[2];
//const auto seed = md5_pseudo_random(ucmd->command_number) & 0xFF;
//interfaces::random->set_seed(seed);
//
//random[0] = interfaces::random->random_float(-0.5f, 0.5f)
//+ interfaces::random->random_float(-0.5f, 0.5f);
//
//random[1] = interfaces::random->random_float(-0.5f, 0.5f)
//+ interfaces::random->random_float(-0.5f, 0.5f);
//
//auto dir = c_vector(1.0f, 0.0f, 0.0f) + (c_vector(0.0f, -1.0f, 0.0f) * spread * random[0]) + (c_vector(0.0f, 0.0f, 1.0f) * spread * random[1]);
//
//q_angle out = math::get_angle(q_angle(0.f, 0.f, 0.f), dir);
//out = math::fix_angles(out);
//
//ucmd->viewangles += out;

//QAngle SpreadAngle(CUserCmd* cmd)
//{
//	C_GMOD_Player* LocalPlayer = (C_GMOD_Player*)cliententitylist()->GetClientEntity(engine()->GetLocalPlayer());
//
//	Vector SpreadCone = LocalPlayer->GetActiveWeapon()->GetBulletSpread();
//	float Spread = FloatNegate((SpreadCone.x + SpreadCone.y + SpreadCone.z) / 3);
//
//	float Random[2];
//	unsigned int seed = MD5_PseudoRandom(cmd->command_number) & 0xFF;
//	Interfaces::Random()->SetSeed(seed);
//
//	Random[0] = Interfaces::Random()->RandomFloat(-0.5f, 0.5f) + Interfaces::Random()->RandomFloat(-0.5f, 0.5f);
//	Random[1] = Interfaces::Random()->RandomFloat(-0.5f, 0.5f) + Interfaces::Random()->RandomFloat(-0.5f, 0.5f);
//
//	Vector ShootDirection = Vector(1.0f, 0.0f, 0.0f) + (Vector(0.0f, -1.0f, 0.0f) * Spread * Random[0]) + (Vector(0.0f, 0.0f, 1.0f) * Spread * Random[1]); // 0,0,0
//
//	QAngle out = H::Util::Math::GetAngle(Vector(0, 0, 0), ShootDirection);
//	out = H::Util::Math::FixAngles(out);
//	return out;
//}