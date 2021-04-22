#include "bgwindow.h"

#include <chrono>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/im_tools.h>

#include "../../../interfaces.h"
#include "../../../settings/settings.h"
#include "../../../utils/hack_utils.h"

std::string current_date_time()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	return buf;
}

bool show_style_editor = false;
bool show_colors_editor = false;
bool show_entity_list = false;

void draw_entity_list()
{
	using namespace ImGui;
	static settings::visuals::c_entity_list ent_list;
	static auto update_time_stamp = 0.f;

	auto is_update = [&]()
	{
		if (update_time_stamp == 0.f)
			return true;

		const auto current_time_stamp = interfaces::engine->get_time_stamp_from_start();

		if (roundf(current_time_stamp) - roundf(update_time_stamp) < 5)
			return false;

		return true;
	};
	
	auto update = [&]()
	{
		if (!is_update())
			return false;

		bg_window::is_entlist_updating = true;
		
		update_time_stamp = interfaces::engine->get_time_stamp_from_start();
		
		ent_list.clear();
	
		for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i)
		{
			auto* ent = get_entity_by_index(i);
			if (!ent || interfaces::entity_list->get_entity_by_handle(ent->get_owner_entity_handle()) != nullptr)
				continue;

			auto print_name = ent->get_print_name();

			if (ent_list.exist(print_name))
				continue;

			ent_list.push_back(print_name);
		}

		bg_window::is_entlist_updating = false;
		
		return true;
	};



	
	int w, h;
	interfaces::engine->get_screen_size(w, h);
	ImGui::SetNextWindowSizeConstraints({ 0, 0 }, { w / 2.f, h / 2.f });
	Begin("Entity list##SUBWINDOW");

	static ImGuiTextFilter filter;
	filter.Draw();

	Text("%f and %f", interfaces::engine->get_time_stamp_from_start(), update_time_stamp);
	
	update();
	
	if (ImGui::BeginTable("entitys_table", 2, ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter))
	{	
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("ESP");
		ImGui::TableHeadersRow();
		
		for (auto print_name : ent_list.data())
		{
			if (!filter.PassFilter(print_name.c_str()))
				continue;
		
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", print_name.c_str());
			ImGui::TableNextColumn();
			if (ImGui::Button(settings::visuals::entitys_to_draw.exist(print_name) ? (std::string("Remove##ENTS_TABLE") + print_name).c_str() : (std::string("Add##ENTS_TABLE") + print_name).c_str()))
			{				
				if (settings::visuals::entitys_to_draw.exist(print_name))
					settings::visuals::entitys_to_draw.remove(settings::visuals::entitys_to_draw.find(print_name));
				else
					settings::visuals::entitys_to_draw.push_back(print_name);
			}
			
		}
		ImGui::EndTable();
	}

	End();
}

void bg_window::draw()
{
	int w, h;
	interfaces::engine->get_screen_size(w, h);
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(w + 2, h), ImGuiCond_Always);

	ImGui::Begin("##BGWINDOW", nullptr,
	             ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
	             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Hack"))
		{
			if (ImGui::MenuItem("Exit"))
				hack_utils::shutdown_hack();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Imgui style editor", 0, &show_style_editor);
			ImGui::MenuItem("Colors editor", 0, &show_colors_editor);
			ImGui::MenuItem("Entity list", 0, &show_entity_list);
				
			ImGui::EndMenu();
		}

		//auto current_time = std::chrono::system_clock::now();
		//auto text = current_date_time();
		//auto text_size = ImGui::CalcTextSize(text.c_str());

		//ImGui::GetWindowDrawList()->AddText({ImGui::GetWindowSize().x - 10 - text_size.x, ImGui::GetWindowSize().y / 2 - text_size.y / 2 },
		//ImGui::GetColorU32({ 0.7f, 0.7f, 0.7f, 1.f }), text.c_str());
		//ImGui::SetCursorPos({ImGui::GetWindowSize().x - 10 - text_size.x - 10, 0 });
		//ImGui::Dummy({ImGui::GetWindowSize().x - (ImGui::GetWindowSize().x - 10 - text_size.x - 10), ImGui::GetWindowSize().y });
		
		ImGui::EndMenuBar();
	}

	ImGui::End();

	if (show_style_editor)
	{
		ImGui::Begin("Imgui Style Editor");
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	if (show_colors_editor)
	{
		int w, h;
		interfaces::engine->get_screen_size(w, h);
		ImGui::SetNextWindowSize({ w / 2.f, h / 2.f }, ImGuiCond_Once);
		
		ImGui::Begin("Colors Editor", (bool*)0);
		
		if (ImGui::BeginTable("Colors##COLOREDITOR", 2, ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("Name##COLOREDITOR");
			ImGui::TableSetupColumn("Color##COLOREDITOR");
			ImGui::TableHeadersRow();

			for (auto& i : settings::colors::colors_map)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", i.first.c_str());
				ImGui::TableNextColumn();
				ImGui::ColorEdit4(std::string(std::string("##") + i.first).c_str(), i.second.data());
			}
			
			ImGui::EndTable();
		}

		ImGui::SameLine();
		
		ImGui::End();
	}

	if (show_entity_list)
		draw_entity_list();

	//ImGui::ShowDemoWindow();
}
