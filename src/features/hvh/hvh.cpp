#include "hvh.h"

#include "../../settings/settings.h"
#include "../../interfaces.h"

using namespace interfaces;

void hvh::anti_aims(c_user_cmd* cmd)
{
	if (!settings::states["hvh::anti_aims"])
		return;

	if ((cmd->buttons & IN_ATTACK) || (cmd->buttons & IN_USE))
		return;
	
	auto pitch_type = static_cast<settings::hvh::e_pitch>(settings::values["hvh::pitch_type"]);
	auto yaw_type = static_cast<settings::hvh::e_yaw>(settings::values["hvh::yaw_type"]);

	if (pitch_type != settings::hvh::e_pitch::none)
	{
		switch (pitch_type)
		{
		case settings::hvh::e_pitch::down:
			cmd->viewangles.x = 90.f;
			break;
		case settings::hvh::e_pitch::emotional:
			cmd->viewangles.x = 89.f;
			break;
		case settings::hvh::e_pitch::up:
			cmd->viewangles.x = -90.f;
			break;
			default: break;
		}
	}

	if (yaw_type != settings::hvh::e_yaw::none)
	{
		switch (yaw_type)
		{
		case settings::hvh::e_yaw::forward:
			cmd->viewangles.y = 180.f;
			break;
		case settings::hvh::e_yaw::backward:
			cmd->viewangles.y = -180.f;
			break;			
		default: break;
		}
	}
}

