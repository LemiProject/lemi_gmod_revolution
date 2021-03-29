#pragma once

class iv_render_view
{
public:
	virtual void			unused_0() = 0;
	virtual void			unused_1() = 0;
	virtual void			unused_2() = 0;
	virtual void			unused_3() = 0;
	virtual void			set_blend(float blend) = 0;
	virtual float			get_blend(void) = 0;
	virtual void			set_color_modulation(float const* blend) = 0;
	virtual void			get_color_modulation(float* blend) = 0;
};