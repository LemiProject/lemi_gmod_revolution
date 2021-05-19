#include "math.h"


#include "../interfaces.h"
#include "../game_sdk/interfaces/lua_shared.h"

float math::rad2deg(float x)
{
	return x * (180.f / PI_F);
}

double math::rad2deg(double x)
{
	return x * (static_cast<double>(180.0) / PI_D);
}

float math::deg2rad(float x)
{
	return x * (PI_F / 180.f);
}

double math::deg2rad(double x)
{
	return x * (PI_D / static_cast<double>(180.0));
}

float math::procent(float var, float full)
{
	return 100.f / full * var;
}

double math::procent(double var, double full)
{
	return 100.0 / full * var;
}

float math::float_negative(float var)
{
	return -var;
}

double math::double_negative(double var)
{
	return -var;
}

#undef min
#undef max
double math::lua::rand(double min, double max)
{
	auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
	c_lua_auto_pop ap(glua);
	
	glua->push_special((int)e_special::glob); //1
	glua->get_field(-1, "math"); //2
	glua->get_field(-1, "Rand"); //2
	glua->push_number(min); //3
	glua->push_number(max); //4
	glua->call(2, 1); //3
	auto out = glua->get_number();
	return out;
}

void math::lua::random_seed(double seed)
{
	auto glua = interfaces::lua_shared->get_interface((int)e_special::glob);
	c_lua_auto_pop ap(glua);
	
	glua->push_special((int)e_special::glob); //1
	glua->get_field(-1, "math"); //2
	glua->get_field(-1, "randomseed"); //2
	glua->push_number(seed); //3
	glua->call(1, 0); //2
}
