#pragma once


class i_panel
{
public:
	const char* get_name(int iIndex)
	{
		using orig_fn = const char* (__thiscall*)(i_panel*, int); //36
		return(*(orig_fn**)this)[36](this, iIndex);
	}
};