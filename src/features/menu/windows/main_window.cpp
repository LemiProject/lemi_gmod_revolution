#include "main_window.h"

#include <chrono>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/im_tools.h>
#include <imgui/imgui_internal.h>

#include "tabs_contents.h"

#include "../../../interfaces.h"
#include "../../../render_system/render_system.h"

using namespace ImGui;

constexpr size_t tabs_count = 5;
constexpr const char* const tabs_names[tabs_count] = {
	"LEGIT##TABNAME",
	"RAGE##TABNAME",
	"VISUAL##TABNAME",
	"MISC##TABNAME",
	"SETTINGS##TABNAME",
};
enum e_tabs {
	e_tabs_legit = 0,
	e_tabs_rage,
	e_tabs_visual,
	e_tabs_misc,
	e_tabs_settings
};

e_tabs current_tab = e_tabs_visual;

void draw_top_bar()
{
	auto calc_bar_size = []()
	{
		int x, y;
		interfaces::engine->get_screen_size(x, y);
		if (x < 1280 && y < 720)
			return ImVec2(GetWindowSize().x / 5.8f, 0);
		
		return ImVec2(GetWindowSize().x / 6.2f, 0);
	};

	auto calc_button_size = [](const ImVec2& bar_size)
	{
		return ImVec2(bar_size.x - (GetStyle().WindowPadding.x /*+ GetStyle().ItemSpacing.x*/), GetWindowSize().y / (tabs_count * 2));
	};
	
	if (BeginChild("##TABBAR", calc_bar_size(), false, ImGuiWindowFlags_NoScrollbar))
	{
		PushFont(render_system::fonts::title_font);
		SetCursorPosX((GetWindowSize().x - CalcTextSize("LemiProject").x) / 2);
		TextColored({0, 0.7f, 0.3f, 1.f}, "LemiProject");

		
		
		for (auto i = 0; i < tabs_count; ++i)
		{
			bool current = false;
			if (current_tab == i)
			{
				PushStyleColor(ImGuiCol_Text, { 0, 0.7f, 0.3f, 1.f });
				PushStyleColor(ImGuiCol_Separator, { 0, 0.7f, 0.3f, 1.f });
				current = true;
			}
			
			const auto name = tabs_names[i];
			if (TopBarButton(name, calc_button_size(GetWindowSize()), CustomColor_t({ 1, 1, 1, 1 })))
				current_tab = static_cast<e_tabs>(i);
			
			if (current) PopStyleColor(2);
		}

		PopFont();
	}
	EndChild();
	GetWindowDrawList()->AddLine({GetItemRectMax().x, GetItemRectMin().y}, GetItemRectMax(),
	                            GetColorU32(GetStyle().Colors[ImGuiCol_Separator]), 2);
}


void draw_tab_top()
{
	PushFont(render_system::fonts::title_font);

	//SetCursorPos({ 0, 0 });
	Dummy({ GetWindowSize().x, GetTitleBarHeight() });
	//BorderPrevItem();

	//GetWindowDrawList()->AddRectFilled({ 0, 0 }, { GetWindowSize().x, GetTitleBarHeight() },
	//	GetColorU32(ImGuiCol_TitleBg));
	auto rect_min = GetItemRectMin();
	const auto rect_max = GetItemRectMax();
	auto rect_size = GetItemRectSize();

	auto text_size = CalcTextSize(tabs_names[current_tab], 0, true);

	auto text_min = ImVec2{ rect_min.x + rect_size.x / 2 - text_size.x / 2 - GetStyle().FramePadding.x, rect_min.y + rect_size.y / 2 - text_size.y / 2 };
	auto text_max = ImVec2{ rect_min.x + rect_size.x / 2 + text_size.x - GetStyle().FramePadding.x, rect_min.y + rect_size.y / 2 + text_size.y };

	RenderTextClipped(text_min, text_max, tabs_names[current_tab], FindRenderedTextEnd(tabs_names[current_tab]), &text_size);

	//GetWindowDrawList()->AddRectFilled(text_min, text_max, GetColorU32({ 1, 0, 0, 1 }));
	PopFont();
}

void main_window::draw()
{
	if (!show_main_window)
		return;
	
	static auto calc_window_size = []()
	{
		int x, y;
		interfaces::engine->get_screen_size(x, y);
		if (x < 1280 && y < 720)
			return ImVec2(x * 0.8f, y * 0.8f);
		return ImVec2(x / 2, y / 2);
	};

	SetNextWindowSize(calc_window_size(), ImGuiCond_Always);
	PushFont(render_system::fonts::main_font);
	Begin("##MAINWINDOW", static_cast<bool*>(nullptr),
	      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	draw_top_bar();
	SameLine();
	if (BeginChild("##MAINWINDOWCONTENT"))
	{
		draw_tab_top();
		
		switch (current_tab)
		{
		case e_tabs_legit:
			menu_tabs_content::draw_legit_bot();
			break;
		case e_tabs_rage:
			menu_tabs_content::draw_rage_bot();
			break;
		case e_tabs_visual:
			menu_tabs_content::draw_visuals();
			break;
		case e_tabs_misc:
			menu_tabs_content::draw_misc();
			break;
		case e_tabs_settings:
			menu_tabs_content::draw_setting();
			break;
		}
	}
	EndChild();
	
	End();
	PopFont();
}
