#include "math.h"

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