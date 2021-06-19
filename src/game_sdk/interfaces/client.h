#pragma once

#include <apiquery2.h>

#include "../misc/c_client_class.h"

class c_vector;
class c_view_setup;

class c_voice_status
{
public:
	virtual ~c_voice_status() = 0;

	virtual int init(void*, void*) = 0;
	virtual void vid_init() = 0;
};

class c_client	//TODO: COMPLETE THIS https://i.imgur.com/ut06Dyc.png
{
public:
	virtual int init(void* app_system_factory, void* physics_factory, void* globals) = 0;
	virtual void post_init() = 0;
	virtual void shutdown() = 0;
	virtual bool replay_init(void* reply_factory) = 0;
	virtual bool replay_post_init() = 0;
	virtual void level_init_pre_entity(const char* name) = 0;
	virtual void level_init_post_entity() = 0;
	virtual void level_shutdown() = 0;
	virtual c_client_class* get_client_class() = 0;
	virtual int	hud_vid_init(void) = 0;
	virtual void hud_process_input(bool bActive) = 0;
	virtual void hud_update(bool bActive) = 0;
	virtual void hud_reset(void) = 0;
	virtual void hud_text(const char* message) = 0;
	virtual bool in_activate_mouse() = 0;
	virtual bool in_deactivate_mouse() = 0;
	virtual bool in_accumulate() = 0;
	virtual bool in_clear_states() = 0;
	virtual bool in_is_key_down(const char*, bool& is_down) = 0;
	virtual void in_on_mouse_wheeled(int delta) = 0;
	virtual int in_key_event(int event_code, void* key_num, const char* current_binding) = 0;
	virtual void create_move(int sequence_number, float frame_time, bool active) = 0;
	virtual void extra_mouse_sample(float frame_time, bool active) = 0;
	virtual bool write_usercmd_delta_to_buffer(void* buf, int from, int to, bool is_new_command) = 0;//TODO: FIX THIS (void*)
	virtual void encode_user_cmd_to_buffer(void* buf, int slot) = 0;
	virtual void decode_user_cmd_from_buffer(void* buf, int slot) = 0;
	virtual void view_render(void* rect) = 0;
	virtual void render_view(const c_view_setup& view, int clear_flags, int what_to_draw) = 0;
	virtual void view_fade(void* p_sf) = 0;
	virtual void set_crosshair_angle(const c_vector& angle) = 0;
	virtual void init_sprite(void* p_sprite, const char* load_name) = 0;
	virtual void shutdown_sprite(void* p_sprite) = 0;
	virtual int	 get_sprite_size(void) const = 0;
	virtual void voice_status(int ent_index, void* talking) = 0;
	virtual void install_string_table_callback(const char* table_name) = 0;
	virtual void frame_stage_notify(void* cur_stage) = 0;
	virtual bool dispatch_user_message(int msg_type, void* msg_data) = 0;
	virtual void* save_init(int size) = 0;
	virtual void			save_write_fields(void*, const char*, void*, void*, void*, int) = 0;
	virtual void			save_read_fields(void*, const char*, void*, void*, void*, int) = 0;
	virtual void			pre_save(void*) = 0;
	virtual void			save(void*) = 0;
	virtual void			write_save_headers(void*) = 0;
	virtual void			read_restore_headers(void*) = 0;
	virtual void			restore(void*, bool) = 0;
	virtual void			dispatch_on_restore() = 0;
	virtual void* get_standard_recv_proxies() = 0;
	virtual void			write_save_game_screenshot(const char* filename) = 0;
	virtual void			emit_sentence_close_caption(char const* token_stream) = 0;
	virtual void			emit_close_caption(char const* caption_name, float duration) = 0;
	virtual bool			can_record_demo(char* error_msg, int length) const = 0;
	virtual void			on_demo_record_start(char const* demo_base_name) = 0;
	virtual void			on_demo_record_stop() = 0;
	virtual void			on_demo_playback_start(char const* demo_base_name) = 0;
	virtual void			on_demo_playback_stop() = 0;
	virtual bool			should_draw_dropdown_console() = 0;
	virtual int				get_screen_width() = 0;
	virtual int				get_screen_height() = 0;
	virtual void			write_save_game_screenshot_of_size(const char* p_filename, int width, int height, bool b_create_power_of2_padded/*=false*/, bool b_write_vtf/*=false*/) = 0;
	virtual bool			get_player_view(c_view_setup& player_view) = 0; //59
	virtual void			setup_game_properties(void* contexts, void* properties) = 0;
	virtual UINT			get_presence_id(const char* p_id_name) = 0;
	virtual const char* get_property_id_string(const UINT id) = 0;
	virtual void			get_property_display_string(UINT id, UINT value, char* p_output, int n_bytes) = 0;
	virtual void			invalidate_mdl_cache() = 0;
	virtual void			in_set_sample_time(float frame_time) = 0;
	virtual void			reload_files_in_list(void* p_files_to_reload) = 0;
	virtual void			start_stats_reporting(HANDLE handle, bool b_arbitrated) = 0;
	virtual bool			handle_ui_toggle() = 0;
	virtual bool			should_allow_console() = 0;
	virtual void* get_renamed_recv_table_infos() = 0;
	virtual void* get_client_ui_mouth_info() = 0;
	virtual void	file_received(const char* file_name, unsigned int transferID) = 0;
	virtual const char* translate_effect_for_vision_filter(const char* pchEffectType, const char* pchEffectName) = 0;
	
};
