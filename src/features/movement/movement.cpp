#include "movement.h"

#include "../../game_sdk/entitys/c_base_player.h"
#include "../../settings/settings.h"
#include "../../utils/game_utils.h"

void movement::bunny_hop::run_bunny_hop(c_user_cmd& ucmd)
{
	if (!settings::states["misc::bunny_hop"])
        return;
	
    auto* local_player = get_local_player();
    static bool last_jumped = false, should_fake = false;

    if (!last_jumped && should_fake)
    {
        should_fake = false;
        ucmd.buttons |= IN_JUMP;
    }
    else if (ucmd.buttons & IN_JUMP)
    {
        if (local_player->get_flags() & (1 << 0))
        {
            last_jumped = true;
            should_fake = true;
        }
        else
        {
            ucmd.buttons &= ~IN_JUMP;
            last_jumped = false;
        }
    }
    else
    {
        last_jumped = false;
        should_fake = false;
    }

    if (settings::states["lua::hack_hooks"])
        lua::hook_call("LPostBhop");
}

void movement::bunny_hop::run_auto_strafe(c_user_cmd& cmd)
{
    if (!settings::states["misc::auto_strafe"])
        return;
	
    c_base_player* local_player = get_local_player();

    if (!local_player || !local_player->is_alive())
        return;

    if (local_player->get_move_type() == (int)e_move_type::ladder)
        return;

    if (!(cmd.buttons & IN_JUMP) || !(local_player->get_flags() & (1 << 0)))
        return;

    c_vector view_angles;
    interfaces::engine->get_view_angles(view_angles);

    static bool side_switch = false;
    side_switch = !side_switch;

    cmd.forwardmove = 0.f;
    cmd.sidemove = side_switch ? 450.f : -450.f;

    float velocity_yaw = local_player->get_velocity().y;

    float rotation = std::clamp< float >(math::rad2deg(std::atan2(15.f, local_player->get_velocity().length2d())), 0.f, 90.f);

    float abs_yaw = std::fabs(math::normalize_angle(velocity_yaw - view_angles.y));

    float ideal_yaw_rotation = (side_switch ? rotation : -rotation) + (abs_yaw < 5.f ? velocity_yaw : view_angles.y);

    float cos_rot = std::cos(math::deg2rad(view_angles.y - ideal_yaw_rotation));
    float sin_rot = std::sin(math::deg2rad(view_angles.y - ideal_yaw_rotation));

    cmd.forwardmove = (cos_rot * cmd.forwardmove) - (sin_rot * cmd.sidemove);
    cmd.sidemove = (sin_rot * cmd.forwardmove) + (cos_rot * cmd.sidemove);
}
