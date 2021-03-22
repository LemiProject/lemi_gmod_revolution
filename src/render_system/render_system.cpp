#include "render_system.h"
#include "../utils/memory_utils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>

#include "../features/menu/menu.h"
#include "../features/visuals/visuals.h"

#include "fonts/lemi_main_font.h"
#include "img/logo.h"
#include "img/img_loader.h"

#include "../utils/hack_utils.h"

#include <d3dx9.h>

IDirect3DDevice9* device;

void render_system::init()
{
	device = **(reinterpret_cast<IDirect3DDevice9***>(memory_utils::pattern_scanner(
		"shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1));

	ImGui::CreateContext();

	auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
	if (game_hwnd)
	{
		ImGui_ImplWin32_Init(game_hwnd);
		ImGui_ImplDX9_Init(device);

		ImGui::GetIO().IniFilename = nullptr;
		fonts::main_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(lemi_fonts::lemi_main_font_compressed_data,
			lemi_fonts::lemi_main_font_compressed_size, 21, nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
		fonts::title_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(lemi_fonts::lemi_main_font_compressed_data,
			lemi_fonts::lemi_main_font_compressed_size, 26, nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
		fonts::in_game_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(lemi_fonts::lemi_main_font_compressed_data,
			lemi_fonts::lemi_main_font_compressed_size, 16, nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	}
	
	menu::init();
}

void render_system::shutdown()
{

}

//void clear_screen_for_menu()
//{
//	if (menu::menu_is_open())
//	{
//		//const auto rect_color = D3DCOLOR_XRGB(255, 0, 0);
//		//D3DRECT bar_rect = { 1, 1, 1, 1 };
//
//		//device->Clear(1, &bar_rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rect_color, 0, 0);
//		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
//		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
//		device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
//		
//	}
//}

void render_system::on_scene_end()
{
	IDirect3DStateBlock9* pixel_state = NULL;
	IDirect3DVertexDeclaration9* vertex_declaration;
	IDirect3DVertexShader9* vertex_shader;
	device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
	device->GetVertexDeclaration(&vertex_declaration);
	device->GetVertexShader(&vertex_shader);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

	DWORD state1;
	DWORD state2;
	DWORD state3;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &state1);
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &state2);
	device->GetRenderState(D3DRS_SRGBWRITEENABLE, &state3);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	menu::draw();

	visuals::run_visuals();
	
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetRenderState(D3DRS_COLORWRITEENABLE, state1);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, state2);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, state3);
	
	pixel_state->Apply();
	pixel_state->Release();
	device->SetVertexDeclaration(vertex_declaration);
	device->SetVertexShader(vertex_shader);
}

IDirect3DDevice9* render_system::get_device()
{
	return device;
}
