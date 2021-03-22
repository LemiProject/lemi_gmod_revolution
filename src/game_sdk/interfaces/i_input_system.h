#pragma once


class i_input_system
{
public:
	void enable_input(bool state)
	{
		using fn = void(__thiscall*)(i_input_system*, bool);
		return (*(fn**)this)[7](this, state);
	}
};