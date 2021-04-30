// ReSharper disable CppInconsistentNaming
#pragma once
#include <ostream>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

inline std::ostream& operator<<(std::ostream& os, const ImVec2& vec)
{
	os << "x:\t" << vec.x << "\ty:" << vec.y;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const ImVec4& vec)
{
	os << "x:\t" << vec.x << "\ty:" << vec.y << "\tz" << vec.z << "\tw" << vec.w;
	return os;
}

namespace ImGui
{
	struct CustomColor_t
	{
		ImVec4 color;
		ImVec4 active;
		ImVec4 hovered;

		CustomColor_t(const ImVec4& col) : color(col), active(col), hovered(col) {}
		CustomColor_t(const ImVec4& color, const ImVec4& hovered, const ImVec4& active) : color(color), active(active), hovered(hovered) {}
	};

	IMGUI_API void BorderPrevItem(const ImVec4& color = {1, 0, 0, 1});
	
	IMGUI_API void FullscreenNextWindow();
	
	IMGUI_API void CenterNextElem(size_t size);

	IMGUI_API void BeginPadding(const ImVec2& val);
	IMGUI_API void EndPadding();

	IMGUI_API void BeginCenterVerticalPadding();
	IMGUI_API void EndCenterVerticalPadding();

	IMGUI_API bool IsVectorValid(const ImVec2& val);
	IMGUI_API bool IsVectorValid(const ImVec4& val);

	IMGUI_API bool ColoredButton(const char* text, const ImVec2& size, const CustomColor_t& color);

	IMGUI_API bool TopBarButton(const char* label, const ImVec2& size, const CustomColor_t& color);

	IMGUI_API ImVec4 RGBAToClippedRGBA(const ImVec4& in);
	IMGUI_API void RGBAToClippedRGBA(const ImVec4& in, ImVec4* out);
	IMGUI_API void RGBAToClippedRGBA(const ImVec4& in, ImVec4& out);

	IMGUI_API void BeginTempItemSpacing(const ImVec2& spacing);
	IMGUI_API void EndTempItemSpacing();

	IMGUI_API bool BeginTitledChild(const char* label, const ImVec2& size = {0, 0});
	IMGUI_API void EndTitledChild();

	IMGUI_API float GetTitleBarHeight();

	IMGUI_API void BeginGroupPanel(const char* name, const ImVec2& size, ImU32 color = 0/*, ImU32 text_bg_color*/);
	IMGUI_API void EndGroupPanel();

	IMGUI_API void ToggleButton(const char* str_id, bool* v, const ImVec2& size = ImVec2(0, 0));

	IMGUI_API bool Hotkey(const char* id, uint32_t* key, const ImVec2& size_arg = {0, 0}, uint32_t none_key = 0, const char* none_str = "None");
}
