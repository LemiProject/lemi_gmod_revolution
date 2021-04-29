#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <color.h>

namespace draw_types
{
	struct draw_data_t
	{
		virtual ~draw_data_t() = default;
		virtual void draw(ImDrawList* list) = 0;
	};

	struct rect_draw_type_t final : public draw_data_t
	{
		bool filled;
		ImVec2 min;
		ImVec2 max;
		c_color color;
		float rounding;
		
		void draw(ImDrawList* list) override
		{
			filled ? list->AddRectFilled(min, max, color.get_u32(), rounding) : list->AddRect(min, max, color.get_u32(), rounding);
		}
	};

	struct line_draw_type_t final : public draw_data_t
	{
		ImVec2 pos1;
		ImVec2 pos2;
		c_color color;
		float thickness;
		
		void draw(ImDrawList* list) override
		{
			list->AddLine(pos1, pos2, color.get_u32(), thickness);
		}
	};

	
	struct text_draw_type_t final : public draw_data_t
	{
		ImFont* font;
		std::string text;
		ImVec2 pos;
		float size;
		c_color color;
		int flags;
		
		void draw(ImDrawList* list) override
		{
			enum e_font_flags
			{
				font_none = 0,
				font_centered_x = (1 << 0),
				font_centered_y = (1 << 1),
				font_centered = font_centered_x | font_centered_y,
				font_drop_shadow = (1 << 2),
				font_outline = (1 << 3)
			};

			if (!font->ContainerAtlas)
				return;

			auto text_pos = pos;
			auto text_size = font->CalcTextSizeA(size, FLT_MAX, 0.f, text.c_str());
			flags& font_centered_x ? text_pos.x -= text_size.x * 0.5f : 0;
			flags& font_centered_y ? text_pos.y -= text_size.y * 0.5f : 0;

			list->PushTextureID(font->ContainerAtlas->TexID);

			if (flags & font_drop_shadow) list->AddText(font, size, text_pos, c_color(color.a, color.a, color.a, color.a).get_u32(), text.c_str());
			flags& font_outline ? list->AddTextOutlined(font, text_pos, size, color.get_u32(), colors::black_color.get_u32(), text.c_str()) :
				list->AddText(font, size, text_pos, color.get_u32(), text.c_str());

			list->PopTextureID();
		}
	};

	struct circle_draw_type_t final : public draw_data_t
	{
		bool filled;
		c_color color;
		ImVec2 pos;
		float radius;
		int segment_num;
		float t;
		
		void draw(ImDrawList* list) override
		{
			list->AddCircle(pos, radius, color.get_u32(), segment_num, t);
		}
	};
}
