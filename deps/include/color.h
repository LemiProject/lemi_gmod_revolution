#pragma once
#include <imgui/im_tools.h>
#include <array>

class c_color
{
public:	
	float r{}, g{}, b{}, a{};
	
	c_color();
	c_color(const c_color& c);
	c_color(const c_color&& c) noexcept;
	c_color(float r, float g, float b, float a = 255.f);
	
	explicit c_color(const ImVec4& imvec);
	explicit c_color(const ImVec4&& imvec);  //rvalue impl
	explicit c_color(std::array<float, 4> arr);
	
	~c_color() = default;
	
	float* get_base();
	void init(float r, float g, float b, float a = 255.f);

	c_color& operator=(const c_color& left) = default;
	c_color& operator=(c_color&& left) = default;
	bool operator==(const c_color& left) const;

	[[nodiscard]] ImU32 get_u32() const;
	[[nodiscard]] ImVec4 get_vec4() const;
	[[nodiscard]] std::array<float, 4> get_clamped() const;
};

inline c_color::c_color() = default;


inline c_color::c_color(const c_color& c)
{
	init(c.r, c.g, c.b, c.a);
}

inline c_color::c_color(const c_color&& c) noexcept
{
	init(c.r, c.g, c.b, c.a);
}

inline c_color::c_color(const float r, const float g, const float b, const float a)
{
	init(r, g, b, a);
}

inline c_color::c_color(const ImVec4& imvec)
{
	init(imvec.x * 255, imvec.y * 255, imvec.w * 255, imvec.z * 255);
}

inline c_color::c_color(const ImVec4&& imvec)
{
	init(imvec.x * 255, imvec.y * 255, imvec.w * 255, imvec.z * 255);
}

inline c_color::c_color(std::array<float, 4> arr)
{
	init(arr[0] > 1.f ? arr[0] : arr[0] * 255, arr[1] > 1.f ? arr[1] : arr[1] * 255,
	     arr[2] > 1.f ? arr[2] : arr[2] * 255, arr[3] > 1.f ? arr[3] : arr[3] * 255); //IT`S TIME TO HARD CODE EEEEEE
}

inline c_color color_from_hsv(int h, int s, int v)
{
    double      hh, p, q, t, ff;
    long        i;

    c_color out;
	
    if (s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = v * 255.f;
        out.g = v * 255.f;
        out.b = v * 255.f;
        return out;
    }
    hh = h;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = v;
        break;
    case 5:
    default:
        out.r = v;
        out.g = p;
        out.b = q;
        break;
    }

    out.r *= 255.f;
    out.g *= 255.f;
    out.b *= 255.f;
    out.a = 255.f;
    return out;
}

inline float* c_color::get_base()
{
	return reinterpret_cast<float*>(this); //it will reinterpret as float[4] pointer
}

inline void c_color::init(const float r, const float g, const float b, const float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

inline bool c_color::operator==(const c_color& left) const
{
	return r == left.r && g == left.g && b == left.b && a == left.a;
}

inline ImU32 c_color::get_u32() const
{
	return ImGui::GetColorU32(get_vec4());
}

inline ImVec4 c_color::get_vec4() const
{
	return {r / 255.f, g / 255.f, b / 255.f, a / 255.f};
}

inline std::array<float, 4> c_color::get_clamped() const
{
	return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
}


namespace colors
{
	inline const c_color black_color = c_color( 0, 0, 0, 255 );
	inline const c_color red_color = c_color( 255, 0, 0, 255);
	inline const c_color green_color = c_color( 0, 255, 0, 255);
	inline const c_color blue_color = c_color( 0, 0, 255, 255);
}
