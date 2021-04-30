#define IMGUI_DEFINE_MATH_OPERATORS
#include "im_tools.h"
#include <stack>
#include <imgui/imgui_internal.h>

#include <Windows.h>

void ImGui::BorderPrevItem(const ImVec4& color)
{
	GetWindowDrawList()->AddRect(GetItemRectMin(), GetItemRectMax(), GetColorU32(color));
}

void ImGui::FullscreenNextWindow()
{
	SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
	SetNextWindowSize(GetIO().DisplaySize, ImGuiCond_Always);
}

void ImGui::CenterNextElem(const size_t size)
{
	const auto win_size = GetIO().DisplaySize;
	SetCursorPosX((win_size.x * 0.5f) - static_cast<float>(size) + GetStyle().WindowPadding.x);
}

void ImGui::BeginPadding(const ImVec2& val)
{
	PushStyleVar(ImGuiStyleVar_WindowPadding, val);
}

void ImGui::EndPadding()
{
	PopStyleVar();
}

namespace center_padding_global
{
	std::stack<ImVec2> last_items_size;
}
void ImGui::BeginCenterVerticalPadding()
{
	if (center_padding_global::last_items_size.empty())
		return;

	BeginPadding(center_padding_global::last_items_size.top());
}

void ImGui::EndCenterVerticalPadding()
{
	
}

bool ImGui::IsVectorValid(const ImVec4& val)
{
	return val.x && val.y && val.z && val.w;
}

bool ImGui::ColoredButton(const char* text, const ImVec2& size, const CustomColor_t& color)
{
	PushStyleColor(ImGuiCol_Button, color.color);
	PushStyleColor(ImGuiCol_ButtonActive, color.active);
	PushStyleColor(ImGuiCol_ButtonHovered, color.hovered);

	auto res = Button(text, size);
	
	PopStyleColor();
	PopStyleColor();
	PopStyleColor();

	return res;
}

bool ImGui::TopBarButton(const char* label, const ImVec2& size, const CustomColor_t& color)
{
	auto res = InvisibleButton(label, size);
	const auto rect_min = GetItemRectMin();
	const auto rect_max = GetItemRectMax();
	const auto rect_size = GetItemRectSize();

	auto x_coordinates = ImVec2(rect_min.x + rect_size.x / 12, rect_max.x - rect_size.x / 12);

	//GetWindowDrawList()->AddLine({ x_coordinates.x, rect_min.y }, { x_coordinates.y, rect_min.y },
	//	GetColorU32(GetStyle().Colors[ImGuiCol_Separator]));
	//GetWindowDrawList()->AddLine({ x_coordinates.x, rect_max.y }, { x_coordinates.y, rect_max.y },
	//	GetColorU32(GetStyle().Colors[ImGuiCol_Separator]));
	
	auto text_size = CalcTextSize(label, 0, true);
	const auto end = FindRenderedTextEnd(label, 0);

	bool hovered = IsItemHovered();

	if (hovered)
		PushStyleColor(ImGuiCol_Text, {0, 0.6f, 0.4f, 1});
	
	RenderTextClipped(
		{rect_min.x + (rect_size.x / 2 - text_size.x / 2), rect_min.y + (rect_size.y / 2 - text_size.y / 2)}, {
			rect_min.x + (rect_size.x / 2 + text_size.x / 2), rect_min.y + (rect_size.y / 2 + text_size.y / 2)
		}, label, end, &text_size);

	if (hovered)
		PopStyleColor();

	if (hovered)
		PushStyleColor(ImGuiCol_Separator, { 0, 0.6f, 0.4f, 1 });

	auto y_padding = rect_size.y / 14;
	
	GetWindowDrawList()->AddLine({ rect_min.x + 2, rect_min.y + y_padding - 2 }, { rect_min.x + 2, rect_max.y - y_padding - 2 }, GetColorU32(GetStyle().Colors[ImGuiCol_Separator]), 4);
	GetWindowDrawList()->AddLine({ rect_max.x, rect_min.y + y_padding }, { rect_max.x, rect_max.y - y_padding }, GetColorU32(GetStyle().Colors[ImGuiCol_Separator]), 4);

	if (hovered)
		PopStyleColor();
	
	return res;
}
	

ImVec4 ImGui::RGBAToClippedRGBA(const ImVec4& in)
{
	return {in.x / 255.f, in.y / 255.f, in.z / 255.f, in.w / 255.f};
}

void ImGui::RGBAToClippedRGBA(const ImVec4& in, ImVec4* out)
{
	*out = RGBAToClippedRGBA(in);
}

void ImGui::RGBAToClippedRGBA(const ImVec4& in, ImVec4& out)
{
	RGBAToClippedRGBA(in, out);
}

std::stack<ImVec2> spacings;
void ImGui::BeginTempItemSpacing(const ImVec2& spacing)
{
	spacings.push(GetStyle().ItemSpacing);
	GetStyle().ItemSpacing = spacing;
}

void ImGui::EndTempItemSpacing()
{
	GetStyle().ItemSpacing = spacings.top();
	spacings.pop();
}

bool ImGui::IsVectorValid(const ImVec2& val)
{
	return val.x && val.y;
}

std::stack<ImVec2> titled_children_sizes;

bool ImGui::BeginTitledChild(const char* label, const ImVec2& size)
{
	//auto result = BeginChild(label, size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

	BeginGroup();
	
	auto title_bar_height = GetTextLineHeight() * 1.5f;

	BeginTempItemSpacing({0, 0});
	
	//SetCursorPos({ 0, 0 });
	Dummy({ size.x, title_bar_height });

	EndTempItemSpacing();
	
	const auto rect_min = GetItemRectMin();
	const auto rect_max = GetItemRectMax();
	const auto rect_size = GetItemRectSize();

	auto text_size = CalcTextSize(label, 0, true);
	const auto end = FindRenderedTextEnd(label, 0);
	
	const auto text_rect_min = ImVec2{ rect_min.x + (rect_size.x / 2 - text_size.x / 2), rect_min.y + (rect_size.y / 2 - text_size.y / 2) };
	const auto text_rect_max = ImVec2{ rect_min.x + (rect_size.x / 2 + text_size.x / 2), rect_min.y + (rect_size.y / 2 + text_size.y / 2) };
	
	RenderTextClipped(text_rect_min, text_rect_max, label, end, &text_size);

	GetWindowDrawList()->AddLine({ rect_min.x, rect_max.y }, rect_max, GetColorU32(GetStyleColorVec4(ImGuiCol_Separator)));

	Dummy({size.x, GetStyle().ItemSpacing.y});
	SameLine();
	Dummy({ size.x, GetStyle().ItemSpacing.y });
	
	return true;
}

void ImGui::EndTitledChild()
{
	EndGroup();
	BorderPrevItem(GetStyleColorVec4(ImGuiCol_Border));
	//EndChild();
}

float ImGui::GetTitleBarHeight()
{
	return GetTextLineHeight() + GetStyle().FramePadding.y * 2.0f;
}


std::stack<ImU32> begin_groups_colors_stack;

void ImGui::BeginGroupPanel(const char* name, const ImVec2& size, ImU32 color/*, ImU32 text_bg_color*/)
{
	if (color == 0)
		color = GetColorU32(GetStyleColorVec4(ImGuiCol_ChildBg));
	
	begin_groups_colors_stack.push(color);

	//GetWindowDrawList()->ChannelsSplit(2);
	//GetWindowDrawList()->ChannelsSetCurrent(1);

	BeginGroup();

	const auto cursor_pos = GetCursorScreenPos();
	const auto item_spacing = GetStyle().ItemSpacing;
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	const auto frame_height = GetFrameHeight();
	BeginGroup();

	auto effective_size = size;
	if (size.x < 0.0f)
		effective_size.x = GetContentRegionAvailWidth();
	else
		effective_size.x = size.x;
	Dummy(ImVec2(effective_size.x, 0.0f));

	Dummy(ImVec2(frame_height * 0.5f, 0.0f));
	SameLine(0.0f, 0.0f);
	BeginGroup();
	Dummy(ImVec2(frame_height * 0.5f, 0.0f));
	const auto text_size = CalcTextSize(name, 0, true);
	//GetWindowDrawList()->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + effective_size.x, cursor_pos.y + text_size.y), text_bg_color.);
	SetCursorPos(ImVec2(
		GetCursorPos().x + ((effective_size.x - frame_height) / 2 - text_size.x / 2)
		/*- GetStyle().ItemSpacing.x*/, GetCursorPos().y));
	TextUnformatted(name, FindRenderedTextEnd(name));
	//RenderText(ImVec2(GetCursorPos().x + effective_size.x / 2 - text_size.x / 2, GetCursorPos().y), name);
	SameLine(0.0f, 0.0f);
	Dummy(ImVec2(0.0, frame_height + item_spacing.y));
	BeginGroup();

	PopStyleVar(2);

	GetCurrentWindow()->ContentRegionRect.Max.x -= frame_height * 0.5f;
	GetCurrentWindow()->Size.x -= frame_height;

	PushItemWidth(effective_size.x - frame_height);
}

void ImGui::EndGroupPanel()
{
	PopItemWidth();

	const auto item_spacing = GetStyle().ItemSpacing;

	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	const auto frame_height = GetFrameHeight();

	EndGroup();

	//ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

	EndGroup();

	SameLine(0.0f, 0.0f);
	Dummy(ImVec2(frame_height * 0.5f, 0.0f));
	Dummy(ImVec2(0.0, frame_height - frame_height * 0.5f - item_spacing.y));

	EndGroup();

	const auto item_min = GetItemRectMin();
	const auto item_max = GetItemRectMax();

	PopStyleVar(2);

	GetCurrentWindow()->ContentRegionRect.Max.x += frame_height * 0.5f;
	GetCurrentWindow()->Size.x += frame_height;

	Dummy(ImVec2(0.0f, 0.0f));

	EndGroup();

	//GetWindowDrawList()->ChannelsSetCurrent(0);
	GetWindowDrawList()->AddRect(item_min, item_max, GetColorU32(GetStyleColorVec4(ImGuiCol_Border)));
	//GetWindowDrawList()->ChannelsMerge();

	begin_groups_colors_stack.pop();
}

void ImGui::ToggleButton(const char* str_id, bool* v, const ImVec2& size)
{
	const auto p = GetCursorScreenPos();
	//p += ImVec2(10, 10);

	auto draw_list = GetWindowDrawList();
	auto style = GetStyle();
	
	const auto calc_size = CalcItemSize(size, style.FramePadding.x * 2.0f, GetTextLineHeight() + style.FramePadding.y * 2.0f);

	InvisibleButton(str_id, calc_size);
	if (IsItemClicked())
		*v = !*v;
	
	const auto radius = GetItemRectSize().y * 0.5f;
	const auto width = GetItemRectSize().x;
	const auto height = GetItemRectSize().y;

	auto t = *v ? 1.0f : 0.0f;

	auto& g = *GImGui;

	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		const auto t_anim = ImSaturate(g.LastActiveIdTimer / /*ANIM_SPEED*/ 0.2);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	const auto col_bg = GetColorU32(ImGuiCol_ChildBg);

	ImU32 slider_color;
	if (*v)
		slider_color = GetColorU32(ImGuiCol_ButtonActive);
	else
		slider_color = GetColorU32(ImGuiCol_Button);

	draw_list->AddRect(p, ImVec2(p.x + width, p.y + height), GetColorU32(ImGuiCol_Border), height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, slider_color);

}

constexpr const char* const key_names[] = {
	"Unknown",
	"Left Button",
	"Right Button",
	"Cancel",
	"Middle Button",
	"MButton 1",
	"MButton 2",
	"Unknown",
	"Back",
	"Tab",
	"Unknown",
	"Unknown",
	"Clear",
	"Return",
	"Unknown",
	"Unknown",
	"Shift",
	"Control",
	"Menu",
	"Pause",
	"Capital",
	"Kana",
	"Unknown",
	"Junja",
	"Final",
	"Kanji",
	"Unknown",
	"Escape",
	"Convert",
	"Nonconvert",
	"Accept",
	"Modechange",
	"Space",
	"Prior",
	"Next",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"Select",
	"Print",
	"Execute",
	"Snapshot",
	"Insert",
	"Delete",
	"Help",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"LWIN",
	"RWIN",
	"APPS",
	"Unknown",
	"Sleep",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"MULTIPLY",
	"ADD",
	"SEPARATOR",
	"SUBTRACT",
	"DECIMAL",
	"DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"NUMLOCK",
	"SCROLL",
	"OEM_NEC_EQUAL",
	"OEM_FJ_MASSHOU",
	"OEM_FJ_TOUROKU",
	"OEM_FJ_LOYA",
	"OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LShift",
	"RShift",
	"LControl",
	"RControl",
	"LALT",
	"RALT"
};

bool ImGui::Hotkey(const char* label, uint32_t* k, const ImVec2& size_arg, uint32_t none_key, const char* none_str)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = ImGui::FocusableItemRegister(window, id);

	const bool hovered = ImGui::ItemHoverable(frame_bb, id);

	if (hovered) {
		ImGui::SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool user_clicked = hovered && io.MouseClicked[0];

	if (focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			// Start edition
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			*k = 0;
		}
		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (io.MouseClicked[0]) {
		// Release focus when we click outside
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	bool value_changed = false;
	int key = *k;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io.MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
					break;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}
		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io.KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (IsKeyPressedMap(ImGuiKey_Escape)) {
			*k = none_key;
			ImGui::ClearActiveID();
		}
		else {
			*k = key;
		}
	}

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[64] = "None";
	strcpy_s(buf_display, none_str);

	ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, /*ImGui::GetColorU32(ImVec4(0.20f, 0.25f, 0.30f, 1.0f)*/ ImGui::GetColorU32(ImGuiCol_Button), true, style.FrameRounding);

	if (*k != 0 && g.ActiveId != id) {
		strcpy_s(buf_display, key_names[*k]);
	}
	else if (g.ActiveId == id) {
		strcpy_s(buf_display, "<Press a key>");
	}

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
	//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
	//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}
