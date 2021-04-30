#include "bgwindow.h"

#include <chrono>
#include <future>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <imgui/imgui.h>
#include <imgui/im_tools.h>
#include <imgui/TextEditor.h>
#include <imgui/imgui_stdlib.h>



#include "main_window.h"
#include "../../../interfaces.h"
#include "../../../settings/settings.h"
#include "../../../utils/hack_utils.h"
#include "../../lua_features/lua_features.h"

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
bool show_glua_loader = false;

auto entity_list_update_time_stamp = 0.f;
settings::visuals::c_entity_list ent_list;

void bg_window::update_entity_list()
{
	auto is_update = [&]()
	{		
		if (entity_list_update_time_stamp == 0.f)
			return true;

		const auto current_time_stamp = interfaces::engine->get_time_stamp_from_start();

		if (roundf(current_time_stamp) - roundf(entity_list_update_time_stamp) < 1)
			return false;

		return true;
	};

	if (!is_update())
		return;

	is_entlist_updating = true;

	entity_list_update_time_stamp = interfaces::engine->get_time_stamp_from_start();

	if (!interfaces::engine->is_in_game())
		return;
	
	ent_list.clear();

	for (auto i = 0; i < interfaces::entity_list->get_highest_entity_index(); ++i)
	{
		auto* ent = get_entity_by_index(i);
		if (!ent || interfaces::entity_list->get_entity_by_handle(ent->get_owner_entity_handle()) != nullptr)
			continue;

		auto class_name = ent->get_class_name();

		if (ent_list.exist(class_name))
			continue;

		if (class_name.empty())
			continue;
		
		ent_list.push_back(class_name);
	}

	is_entlist_updating = false;
}


void draw_entity_list()
{
	using namespace ImGui;
	
	int w, h;
	interfaces::engine->get_screen_size(w, h);
	ImGui::SetNextWindowSize({ w / 2.f, h / 2.f }, ImGuiCond_FirstUseEver);
	Begin("Entity list##SUBWINDOW");

	static ImGuiTextFilter filter;
	filter.Draw();

	//Text("%f and %f", interfaces::engine->get_time_stamp_from_start(), entity_list_update_time_stamp);
	
	if (ImGui::BeginTable("entitys_table", 2, ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter))
	{	
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("ESP");
		ImGui::TableHeadersRow();

		if (!bg_window::is_entlist_updating)
		{
			for (auto class_name : ent_list.data())
			{
				if (!filter.PassFilter(class_name.c_str()))
					continue;

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", class_name.c_str());
				ImGui::TableNextColumn();
				if (ImGui::Button(settings::visuals::entitys_to_draw.exist(class_name) ? (std::string("Remove##ENTS_TABLE") + class_name).c_str() : (std::string("Add##ENTS_TABLE") + class_name).c_str()))
				{
					if (settings::visuals::entitys_to_draw.exist(class_name))
						settings::visuals::entitys_to_draw.remove(settings::visuals::entitys_to_draw.find(class_name));
					else
						settings::visuals::entitys_to_draw.push_back(class_name);
				}

			}
		}
		ImGui::EndTable();
	}

	End();
}

void draw_glua_loader()
{
	using namespace ImGui;

	static TextEditor* editor = nullptr;
	static auto load_file_show = false;
	if (load_file_show)
	[]()
	{
		static std::string str;
		
		Begin("Load file##SUBWINDOW");

		InputText("Path##LOADFILE_SUBWINDOW", &str);
		if (Button("Run##LOADFILE_SUBWINDOW") && !str.empty() && std::filesystem::exists(str)) auto as2 = std::async(std::launch::async, []()
			{
				std::string code;
				{
					std::ifstream stream(str);
					code = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
				}
				lua_features::add_code_to_run(code);
			});
		
		End();
	}();
	
	if (!editor)
	{
		editor = new TextEditor();
		editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
		editor->SetShowWhitespaces(false);
	}
	
	int w, h;
	interfaces::engine->get_screen_size(w, h);
	SetNextWindowSize({ w / 2.f, h / 2.f }, ImGuiCond_FirstUseEver);
	auto cpos = editor->GetCursorPosition();
	Begin("Glua loader##SUBWINDOW", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			
			MenuItem("Load file", 0, &load_file_show);
				
			
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor->IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor->SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor->CanUndo()))
				editor->Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor->CanRedo()))
				editor->Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor->HasSelection()))
				editor->Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor->HasSelection()))
				editor->Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor->HasSelection()))
				editor->Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor->Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor->SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor->GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor->SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor->SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor->SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor->GetTotalLines(),
		editor->IsOverwrite() ? "Ovr" : "Ins",
		editor->CanUndo() ? "*" : " ",
		editor->GetLanguageDefinition().mName.c_str());

	editor->Render("TextEditor", {
					   ImGui::GetContentRegionAvail().x,
					   ImGui::GetContentRegionAvail().y - (ImGui::CalcTextSize("Run").y + ImGui::CalcTextSize("Run").y / 2)
		});

	//VERY VERY ("VERY" * 30) BAD CODE TODO: FIX IT
	if (Button("Run##RUN_SCRIPT")) auto as = std::async(std::launch::async, []()
	{
			std::string code;
			auto lines = editor->GetTextLines();
			std::for_each(lines.begin(), lines.end(), [&](const std::string& n)
			{
					code.append(" " + n);
			});
			lua_features::add_code_to_run(code);
	});
	
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
			ImGui::MenuItem("Main window", 0, &main_window::show_main_window);
			ImGui::MenuItem("Imgui style editor", 0, &show_style_editor);
			ImGui::MenuItem("Colors editor", 0, &show_colors_editor);
			ImGui::MenuItem("Entity list", 0, &show_entity_list);
			ImGui::MenuItem("Glua loader", 0, &show_glua_loader);
			
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

	if (show_glua_loader)
		draw_glua_loader();
	
	//ImGui::ShowDemoWindow();
}