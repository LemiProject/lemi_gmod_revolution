#include "interfaces.h"
#include "utils/memory_utils.h"


#ifdef _X64_
constexpr auto move_helper_pattern = "48 8B 0D ? ? ? ? 48 8B 53 10";
constexpr auto client_mode_pattern = "48 8B 0D ? ? ? ? 48 8B 01 48 FF 60 50 CC CC 48 83 EC 28";
#else
constexpr auto move_helper_pattern = "8B 0D ? ? ? ? 8B 46 08 68";
#endif

uintptr_t get_address_from_instruction(uintptr_t address, int offs, int i_size)
{
	uintptr_t ins = address + offs;
	int ra = *(uint32_t*)(ins);
	uintptr_t realaddress = address + i_size + ra;
	return realaddress;
}

template<typename t>
t* get_vmt_from_addr(uintptr_t addr, uintptr_t off)
{
	uintptr_t step = 3, ins_size = 7, ins = addr + off;
	uintptr_t imga = *(DWORD*)(ins + step);
	uintptr_t real_address = ins + ins_size + imga;
	return *(t**)(real_address);
}

template <typename T>
static constexpr auto relative_to_absolute(uintptr_t address) noexcept
{
	return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

void interfaces::init_interfaces()
{
	engine = memory_utils::capture_interface<c_engine_client>("engine.dll", "VEngineClient015");
	client = memory_utils::capture_interface<c_client>("client.dll", "VClient017");
	entity_list = memory_utils::capture_interface<v_client_entity_list>("client.dll", "VClientEntityList003");
	surface = memory_utils::capture_interface<i_surface>("vguimatsurface.dll", "VGUI_Surface030");
	input_system = memory_utils::capture_interface<i_input_system>("inputsystem.dll", "InputSystemVersion001");
	lua_shared = memory_utils::capture_interface<c_lua_shared>("lua_shared.dll", "LUASHARED003");
	material_system = memory_utils::capture_interface<i_material_system>("materialsystem.dll", "VMaterialSystem080");
	model_render = memory_utils::capture_interface<iv_model_render>("engine.dll", "VEngineModel016");
	render_view = memory_utils::capture_interface<iv_render_view>("engine.dll", "VEngineRenderView014");
	prediction = memory_utils::capture_interface<i_prediction>("client.dll", "VClientPrediction001");
	engine_trace = memory_utils::capture_interface<i_engine_trace>("engine.dll", "EngineTraceClient003");
	panel = memory_utils::capture_interface<i_panel>("vgui2.dll", "VGUI_Panel009");
	game_movement = memory_utils::capture_interface<i_game_movement>("client.dll", "GameMovement001");

	//https://i.imgur.com/93NQMj4.png
	move_helper = **relative_to_absolute<i_move_helper***>((uintptr_t)memory_utils::pattern_scanner("client.dll", move_helper_pattern) + 0x3);
	
	render_context = material_system->get_render_context();
	client_mode = **relative_to_absolute<i_client_mode***>((uintptr_t)memory_utils::pattern_scanner("client.dll", client_mode_pattern) + 0x3);
	
	https://i.imgur.com/sz8AqEJ.png
	global_vars = *relative_to_absolute<c_global_vars**>((*(uintptr_t**)client)[11] + 16);
	view_render = **reinterpret_cast<i_view_render***>((*reinterpret_cast<uintptr_t**>(client))[27] + 0x5);
	
	void(*random_seed)(int);
	*(void**)&random_seed = GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed");
	if (random_seed)
		random = **(c_uniform_random_stream***)((uintptr_t)random_seed + 0x5);
}