#include "movement.h"

#include "../../game_sdk/entitys/c_base_player.h"
#include "../../settings/settings.h"

void movement::bunny_hop::run_bunny_hop(c_user_cmd& ucmd)
{
	if (!settings::misc::bunny_hop)
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
}
