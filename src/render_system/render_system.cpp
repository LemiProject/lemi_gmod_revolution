#define IMGUI_DEFINE_MATH_OPERATORS
#include "render_system.h"
#include "../utils/memory_utils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>

#include <imgui/imgui_freetype.h>

#include "../features/menu/menu.h"
#include "../features/visuals/visuals.h"

#include "fonts/lemi_main_font.h"
#include "img/logo.h"
#include "img/img_loader.h"

#include "../utils/hack_utils.h"

#include "render_helpers.h"

#include <d3d9.h>

#include <mutex>

#include <ft2build.h>
#include <intrin.h>

#include FT_FREETYPE_H
#include <freetype/freetype.h>

#include "../settings/settings.h"
#include "../utils/game_utils.h"
#pragma comment(lib,"freetype28.lib")


IDirect3DDevice9* device;
std::mutex render_mutex;

namespace directx_render
{
    inline std::vector<std::shared_ptr<draw_types::draw_data_t>> draw_calls;
    inline std::vector<std::shared_ptr<draw_types::draw_data_t>> safe_draw_calls;
    
    void add_temp_to_draw_list(ImDrawList* target_list);
}


bool render_system::vars::is_screen_grab(float curtime)
{
    return curtime <= last_sg_time + 5.f;
}

void render_system::init()
{
    device = **(reinterpret_cast<IDirect3DDevice9***>(memory_utils::pattern_scanner(
        "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1));

    ImGui::CreateContext();

    auto* const game_hwnd = FindWindowW(L"Valve001", nullptr);
    if (game_hwnd)
    {
        vars::game_hwnd = game_hwnd;
    	
        ImGui_ImplWin32_Init(game_hwnd);
        ImGui_ImplDX9_Init(device);

        directx_render::init_directx_render();
        
        ImGui::GetIO().IniFilename = nullptr;

        ImFontConfig font_config;

        font_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        
        fonts::main_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(lemi_fonts::lemi_main_font_compressed_data,
            lemi_fonts::lemi_main_font_compressed_size, 21, &font_config,
            ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        fonts::title_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(lemi_fonts::lemi_main_font_compressed_data,
            lemi_fonts::lemi_main_font_compressed_size, 26, &font_config,
            ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        fonts::in_game_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(lemi_fonts::lemi_main_font_compressed_data,
            lemi_fonts::lemi_main_font_compressed_size, 60.f, &font_config,
            ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts);
    }

    surface_render::init_surface_render();
    
    menu::init();
}

void render_system::shutdown()
{

}

void render_system::on_scene_end(uintptr_t ret_address)
{
    static uintptr_t game_overlay_return_address = 0;
    static i_texture* render_target = 0;
	
    if (!game_overlay_return_address)
    {
        MEMORY_BASIC_INFORMATION mi;
        VirtualQuery((LPVOID)ret_address, &mi, sizeof(MEMORY_BASIC_INFORMATION));
        char mn[MAX_PATH];
        GetModuleFileName((HMODULE)mi.AllocationBase, mn, MAX_PATH);
        if (std::string(mn).find("gameoverlay") != std::string::npos)
            game_overlay_return_address = ret_address;
    }
    if (game_overlay_return_address != (uintptr_t)ret_address && !render_target)
        render_target = interfaces::render_context->get_render_target();
	
    if (game_overlay_return_address != (uintptr_t)ret_address && settings::states["other::anti_obs"])
        return;

    if (settings::states["other::anti_obs"])
        interfaces::render_context->set_render_target(render_target);
	
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
	
    auto* list = ImGui::GetBackgroundDrawList();
    directx_render::add_temp_to_draw_list(list);
	
    ImGui::EndFrame();
    ImGui::Render(list);
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    device->SetRenderState(D3DRS_COLORWRITEENABLE, state1);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, state2);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, state3);

    pixel_state->Apply();
    pixel_state->Release();
    device->SetVertexDeclaration(vertex_declaration);
    device->SetVertexShader(vertex_shader);


    if (settings::states["other::anti_obs"])
        interfaces::render_context->set_render_target(render_target);
}

void render_system::on_present()
{
    DWORD colorwrite, srgbwrite;
    IDirect3DVertexDeclaration9* vert_dec = nullptr;
    IDirect3DVertexShader9* vert_shader = nullptr;
    DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
    device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
    device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    //removes the source engine color correction
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

    device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
    device->GetVertexDeclaration(&vert_dec);
    device->GetVertexShader(&vert_shader);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
    device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
    device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    auto* list = ImGui::GetOverlayDrawList();
    directx_render::add_temp_to_draw_list(list);
  
    menu::draw();
    
    ImGui::EndFrame();
    ImGui::Render(list);
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
    device->SetVertexDeclaration(vert_dec);
    device->SetVertexShader(vert_shader);
}

IDirect3DDevice9* render_system::get_device()
{
    return device;
}

void directx_render::init_directx_render()
{

}

void directx_render::render_surface(std::function<void()> draw_surface)
{	
    draw_calls.clear();
    draw_surface();
    
    std::unique_lock<std::mutex> l(render_mutex);
    draw_calls.swap(safe_draw_calls);
}

void directx_render::add_temp_to_draw_list(ImDrawList* target_list)
{
    const std::unique_lock<std::mutex> l(render_mutex);

    for (auto i : safe_draw_calls)
        i->draw(target_list);

    safe_draw_calls.clear();
}


void directx_render::filled_rect(math::box_t box, c_color color, float round)
{
    auto call = std::make_shared<draw_types::rect_draw_type_t>();
    call->color = color;
    call->filled = true;
    call->min = box.get_min().get_im_vec2();
    call->max = box.get_max().get_im_vec2();
    call->rounding = round;
    draw_calls.push_back(call);
}

void directx_render::bordered_rect(math::box_t box, c_color color, float round)
{
    auto call = std::make_shared<draw_types::rect_draw_type_t>();
    call->color = color;
    call->filled = false;
    call->min = box.get_min().get_im_vec2();
    call->max = box.get_max().get_im_vec2();
    call->rounding = round;
    draw_calls.push_back(call);
}

void directx_render::line(const ImVec2& pos1, const ImVec2& pos2, c_color color, float t)
{
    auto call = std::make_shared<draw_types::line_draw_type_t>();
    call->pos1 = pos1;
    call->pos2 = pos2;
    call->color = color;
    call->thickness = t;

    draw_calls.push_back(call);
}

void directx_render::text(ImFont* font, const std::string& text, const ImVec2& pos, float size, c_color color,
                          int flags)
{
    auto call = std::make_shared<draw_types::text_draw_type_t>();
    call->flags = flags;
    call->text = text;
    call->pos = pos;
    call->font = font;
    call->size = size;
    call->color = color;
    draw_calls.push_back(call);
}

void directx_render::outlined_circle(const ImVec2& pos, int radius, c_color color, int segments, int t)
{
    auto call = std::make_shared<draw_types::circle_draw_type_t>();
    call->pos = pos;
    call->radius = radius;
    call->color = color;
    call->t = t;
    call->segment_num = segments;
    call->filled = false;
	
    draw_calls.push_back(call);
}

void directx_render::filled_circle(const ImVec2& pos, int radius, c_color color, int segments, int t)
{
    auto call = std::make_shared<draw_types::circle_draw_type_t>();
    call->pos = pos;
    call->radius = radius;
    call->color = color;
    call->t = t;
    call->segment_num = segments;
    call->filled = true;

    draw_calls.push_back(call);
}

void directx_render::corner_box(math::box_t box, c_color color)
{
    std::array<ImVec2, 4> points = {
    box.get_min().get_im_vec2(),  ImVec2{box.x, box.y + box.h}, box.get_max().get_im_vec2(),  ImVec2{box.x + box.w, box.y}
    };

    auto draw_corner = [](const ImVec2& center, const ImVec2& add, c_color color)
    {
        math::vec2_t inline_padding = { add.x < 0 ? -1 : 1, add.y < 0 ? -1 : 1 };

        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + add.x/* + inline_padding.x*/, center.y + inline_padding.y }, colors::black_color);
        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + inline_padding.x, center.y + add.y/* + inline_padding.y */ }, colors::black_color);

        inline_padding = { -inline_padding.x, -inline_padding.y };

        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + add.x/* + inline_padding.x*/, center.y + inline_padding.y }, colors::black_color);
        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + inline_padding.x, center.y + add.y /*+ inline_padding.y */ }, colors::black_color);

        line(center, { center.x + add.x, center.y }, color);
        line(center, { center.x, center.y + add.y }, color);
    };

    const ImVec2 side_sizes = { box.w / 3.f, box.h / 3.f };

    for (auto i = 0; i < points.size(); ++i)
    {
        auto point = points[i];
        switch (i)
        {
        case 0:
            draw_corner(point, side_sizes, color);
            break;
        case 1:
            draw_corner(point, { side_sizes.x, -side_sizes.y }, color);
            break;
        case 2:
            draw_corner(point, { -side_sizes.x, -side_sizes.y }, color);
            break;
        case 3:
            draw_corner(point, { -side_sizes.x, side_sizes.y }, color);
            break;
        }
    }
}

void surface_render::init_surface_render()
{
    fonts::in_game_font = create_font("Corbel", 11, (int)e_font_flags::fontflag_dropshadow);
}

void surface_render::reload_fonts()
{
    fonts::in_game_font = create_font("Corbel", 11, (int)e_font_flags::fontflag_dropshadow);
}

void surface_render::filled_rect(math::box_t box, c_color color)
{
    interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
    interfaces::surface->draw_filled_rect(box.x, box.y, box.get_max().x, box.get_max().y);
}

void surface_render::bordered_rect(math::box_t box, c_color color)
{
    interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
    interfaces::surface->draw_outline_rect(box.x, box.y, box.get_max().x, box.get_max().y);
}

void surface_render::line(math::vec2_t pos1, math::vec2_t pos2, c_color color)
{
    interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
    interfaces::surface->draw_line(pos1.x, pos1.y, pos2.x, pos2.y);
}

void surface_render::text(math::vec2_t pos, uint64_t font, std::string_view text, c_color color, bool centered)
{
    auto* const surface = interfaces::surface;
    wchar_t temp[128];
    int text_width, text_height;
    if (MultiByteToWideChar(CP_UTF8, 0, text.data(), -1, temp, 128) > 0) {
        surface->set_text_font(font);
        if (centered) {
            surface->get_text_size(font, temp, text_width, text_height);
            surface->set_text_pos(pos.x - text_width / 2, pos.y);
        }
        else
            surface->set_text_pos(pos.x, pos.y);
        surface->set_text_color(color);
        surface->draw_text(temp, wcslen(temp));
    }
}

void surface_render::outlined_circle(math::vec2_t pos, int radius, c_color color, int segments)
{
    auto draw_circle = [](math::vec2_t pos, int r, int seg)
    {
        using fn = void(__thiscall*)(void*, int, int, int, int);
        return (*(fn**)interfaces::surface)[100](interfaces::surface, pos.x, pos.y, r, seg);
    };
    
    interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
    draw_circle(pos, radius, segments);
}

void surface_render::corner_box(math::box_t box, c_color color)
{
    using namespace surface_render;
    
    //left-up left-down right-down left-up
    std::array<math::vec2_t, 4> points = {
        box.get_min(), math::vec2_t{box.x, box.y + box.h}, box.get_max(), math::vec2_t{box.x + box.w, box.y}
    };

    auto draw_corner = [](math::vec2_t center, math::vec2_t add, c_color color)
    {
        math::vec2_t inline_padding = { add.x < 0 ? -1 : 1, add.y < 0 ? -1 : 1 };
        
        line({center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + add.x, center.y + inline_padding.y }, colors::black_color);
        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + inline_padding.x, center.y + add.y}, colors::black_color);

        inline_padding = { -inline_padding.x, -inline_padding.y };
        
        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + add.x, center.y + inline_padding.y }, colors::black_color);
        line({ center.x + inline_padding.x, center.y + inline_padding.y }, { center.x + inline_padding.x, center.y + add.y}, colors::black_color);
        
        line(center, {center.x + add.x, center.y}, color);
        line(center, {center.x, center.y + add.y}, color);
    };

    const math::vec2_t side_sizes = {box.w / 3.f, box.h / 3.f};
    
    for (auto i = 0; i < points.size(); ++i)
    {
        auto point = points[i];
        switch (i)
        {
        case 0:
            draw_corner(point, side_sizes, color);
            break;
        case 1:
            draw_corner(point, {side_sizes.x, -side_sizes.y}, color);
            break;
        case 2:
            draw_corner(point, {-side_sizes.x, -side_sizes.y}, color);
            break;
        case 3:
            draw_corner(point, {-side_sizes.x, side_sizes.y}, color);
            break;
        }
    }
    
}

uint64_t surface_render::create_font(std::string_view win_path, int size, int flags)
{
    uint64_t tmp = interfaces::surface->create_font();
    interfaces::surface->set_font_style(tmp, win_path.data(), size, 500, 0, 0, flags);
    return tmp;
}


c_color parse_color(c_lua_interface* glua)
{
    c_color color;

    if (!glua)
        return c_color();

    c_lua_auto_pop p(glua);
	
    glua->push_string("r");
    glua->get_table(-2);
    int r = glua->get_number(-1);
    glua->pop();

    glua->push_string("g");
    glua->get_table(-2);
    int g = glua->get_number(-1);
    glua->pop();

    glua->push_string("b");
    glua->get_table(-2);
    int b = glua->get_number(-1);
    glua->pop();

    color.init(r, g, b);
    return color;
}

int directx_render::directx_lua_api::directx_lua_api_filled_rect__Imp(c_lua_interface* lua)
{
    c_lua_auto_pop p(lua);
	
    CHECK_TYPE(lua, 1, (int)e_lua_type::type_number, "expected number", 0);
    auto x = lua->get_number(1);

    CHECK_TYPE(lua, 2, (int)e_lua_type::type_number, "expected number", 0);
    auto y = lua->get_number(2);

    CHECK_TYPE(lua, 3, (int)e_lua_type::type_number, "expected number", 0);
    auto w = lua->get_number(3);

    CHECK_TYPE(lua, 4, (int)e_lua_type::type_number, "expected number", 0);
    auto h = lua->get_number(4);

    filled_rect({ (float)x, (float)y, (float)w, (float)h }, current_color);

    return 0;
}

int directx_render::directx_lua_api::directx_lua_api_bordered_rect__Imp(c_lua_interface* lua)
{
    c_lua_auto_pop p(lua);

    CHECK_TYPE(lua, 1, (int)e_lua_type::type_number, "expected number", 0);
    auto x = lua->get_number(1);

    CHECK_TYPE(lua, 2, (int)e_lua_type::type_number, "expected number", 0);
    auto y = lua->get_number(2);

    CHECK_TYPE(lua, 3, (int)e_lua_type::type_number, "expected number", 0);
    auto w = lua->get_number(3);

    CHECK_TYPE(lua, 4, (int)e_lua_type::type_number, "expected number", 0);
    auto h = lua->get_number(4);

    bordered_rect({ (float)x, (float)y, (float)w, (float)h }, current_color);

    return 0;
}


int directx_render::directx_lua_api::directx_lua_api_set_color__Imp(c_lua_interface* lua)
{
    c_lua_auto_pop p(lua);

    CHECK_TYPE(lua, 1, (int)e_lua_type::type_number, "expected number", 0);
    auto r = lua->get_number(1);

    CHECK_TYPE(lua, 2, (int)e_lua_type::type_number, "expected number", 0);
    auto g = lua->get_number(2);

    CHECK_TYPE(lua, 3, (int)e_lua_type::type_number, "expected number", 0);
    auto b = lua->get_number(3);

    CHECK_TYPE(lua, 4, (int)e_lua_type::type_number, "expected number", 0);
    auto a = lua->get_number(4);

    current_color = { (float)r, (float)g, (float)b, (float)a };
}

int directx_render::directx_lua_api::lua_api_is_screen_grab__Imp(c_lua_interface* lua)
{
    lua->push_bool(render_system::vars::_is_screen_grab);
    return 1;
}


void directx_render::directx_lua_api::push_all(c_lua_interface* lua)
{
    lua::push_cfunction(lua, "LFilledRect", directx_lua_api_filled_rect);
    lua::push_cfunction(lua, "LBorderedRect", directx_lua_api_bordered_rect);
    lua::push_cfunction(lua, "LSetDrawColor",directx_lua_api_set_color);
    lua::push_cfunction(lua, "LIsScreenGrab", lua_api_is_screen_grab);

	
}

namespace chams_manager {
	struct entinfo_t
	{
        c_vector origin;
        q_angle angle;
        c_color color;
        i_material* material;
	};
	
    std::map<c_base_entity*, entinfo_t> chams_list;

    std::mutex chams_mutex;

    bool currently_draw_chams = false;
}

void chams_manager::initialize_chams_manager()
{
	
}

void chams_manager::shutdown_chams_manager()
{
	
}

void chams_manager::add_entity(c_base_entity* ent, i_material* mat, c_color col, const c_vector& origin, const q_angle& ang)
{
    if (currently_draw_chams || !ent || !mat)
		return;
	
    //std::unique_lock l(chams_mutex);
    c_vector orig = origin;
    q_angle angle = ang;
	
    if (!orig.is_valid())
        orig = ent->get_origin();
    if (!angle.is_valid())
        angle = ent->get_angles();
	
    chams_list.emplace(ent, entinfo_t{ orig, angle, col, mat });
}

void chams_manager::draw_chams()
{
    //std::unique_lock l(chams_mutex);
	if (chams_list.empty())
        return;

    currently_draw_chams = true;
	
    for (const auto& chams : chams_list) {
	    if (chams.first && chams.second.origin.is_valid() && chams.second.angle.is_valid() && chams.second.material) {
            
            chams.first->set_abs_origin(chams.second.origin);
            chams.first->set_abs_angles(chams.second.angle);

            float oldColor[3];
            float oldBlend;
	    	
            interfaces::model_render->forced_material_override(chams.second.material);
            interfaces::render_view->get_color_modulation(oldColor);
            oldBlend = interfaces::render_view->get_blend();
	    	
            interfaces::render_view->set_color_modulation(chams.second.color.get_clamped().data());
            interfaces::render_view->set_blend(chams.second.color.get_clamped()[3]);
            
	    	
            chams.first->draw_model(0x1);


            interfaces::render_view->set_color_modulation(oldColor);
            interfaces::render_view->set_blend(oldBlend);
	    	
            interfaces::model_render->forced_material_override(nullptr);

	    	
            //chams.first->set_abs_origin(chams.first->get_origin());
            //chams.first->set_abs_angles(chams.first->get_angles());
	    	
            
	    }
    }

    currently_draw_chams = false;
	
    chams_list.clear();
}

bool chams_manager::is_chams_manager_calls()
{
    return currently_draw_chams;
}
