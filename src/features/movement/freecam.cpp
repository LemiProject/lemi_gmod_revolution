#include "movement.h"

#include "../../settings/settings.h"
#include "../../globals.h"

void movement::free_cam::run_free_cam(c_view_setup& setup, c_vector& camera_position)
{
	if (camera_position == c_vector(0.f))
		camera_position = setup.origin;

	auto speed = settings::values["world::free_camera_speed"];

	c_vector forward, right, up;
	math::angle_to_vectors(setup.angles, forward, right, up);

	const auto cmd = globals::game_data::last_cmd;
	
	if (cmd.buttons & IN_SPEED) speed *= 5.f;
	if (cmd.buttons & IN_DUCK) speed *= 0.5f;

	if (cmd.buttons & IN_JUMP) camera_position.z += speed;

	if (cmd.buttons & IN_FORWARD) camera_position += forward * speed;
	if (cmd.buttons & IN_BACK) camera_position -= forward * speed;
	if (cmd.buttons & IN_MOVERIGHT) camera_position += right * speed;
	if (cmd.buttons & IN_MOVELEFT) camera_position -= right * speed;

	if (camera_position.is_valid())
		setup.origin = camera_position;
}
