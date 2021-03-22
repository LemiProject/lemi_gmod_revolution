#include "bgwindow.h"

#include <chrono>

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

	auto get_all_entitys = []()
	{
		std::vector<std::string> out;
		for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i)
		{
			auto ent = get_entity_by_index(i);
			if (!ent || !ent->is_use_lua())
				continue;
			auto str = ent->get_class_name();
			if (!str.empty() && (std::find(out.begin(), out.end(), str) == out.end()))
				out.push_back(str);
		}
		return out;
	};

	auto is_draw = [](std::string_view ent)
	{
		return std::find(settings::visuals::entitys_to_draw.begin(), settings::visuals::entitys_to_draw.end(),
		                 ent.data()) != settings::visuals::entitys_to_draw.end();
	};
	
	static auto is_inited = false;
	static std::vector<std::string> ents;
	
	if (interfaces::engine->is_in_game() && !is_inited)
	{
		ents = get_all_entitys();
		is_inited = true;
	}
	
	Begin("Entity list##SUBWINDOW");

	if (ImGui::BeginTable("entitys_table", 2, ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("ESP");
		ImGui::TableHeadersRow();
		
		for (auto i : ents)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", i.c_str());
			ImGui::TableNextColumn();
			if (ImGui::Button(is_draw(i) ? (std::string("Remove##ENTS_TABLE") + i).c_str() : (std::string("Add##ENTS_TABLE") + i).c_str()))
			{
				auto get_idx = [](std::string_view str)
				{
					return std::find(settings::visuals::entitys_to_draw.begin(), settings::visuals::entitys_to_draw.end(),
						str.data());
				};
				
				if (is_draw(i))
					settings::visuals::entitys_to_draw.erase(get_idx(i));
				else
					settings::visuals::entitys_to_draw.push_back(i);
			}
			
			//ImGui::Text("%s", is_draw(i) ? "true" : "false");
		}
		ImGui::EndTable();
	}

	if (ImGui::Button("Refresh##ENTS_TABLE") && interfaces::engine->is_in_game())
		ents = get_all_entitys();
	
	
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
		//                                    ImGui::GetColorU32({ 0.7f, 0.7f, 0.7f, 1.f }), text.c_str());
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
