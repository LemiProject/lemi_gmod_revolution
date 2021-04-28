#pragma once
#include "../misc/c_view_setup.h"

struct write_replay_screenshot_params_t;
class i_replay_screenshot_system;
class i_view_render
{
public:
	virtual void		init(void) = 0;

	// Clear any systems between levels
	virtual void		level_init(void) = 0;
	virtual void		level_shutdown(void) = 0;

	// Shutdown
	virtual void		shutdown(void) = 0;

	// RENDERING
	// Called right before simulation. It must setup the view model origins and angles here so 
	// the correct attachment points can be used during simulation.	
	virtual void		on_render_start() = 0;

	// Called to render the entire scene
	virtual	void		render(void* rect) = 0;

	// Called to render just a particular setup ( for timerefresh and envmap creation )
	virtual void		render_view(const c_view_setup& view, int nClearFlags, int whatToDraw) = 0;

	// What are we currently rendering? Returns a combination of DF_ flags.
	virtual int get_draw_flags() = 0;

	// MISC
	// Start and stop pitch drifting logic
	virtual void		start_pitch_drift(void) = 0;
	virtual void		stop_pitch_drift(void) = 0;

	// This can only be called during rendering (while within RenderView).
	virtual void* get_frustum() = 0;

	virtual bool		should_draw_brush_models(void) = 0;

	virtual const c_view_setup* get_player_view_setup(void) const = 0;
	virtual const c_view_setup* get_view_setup(void) const = 0;

	virtual void		disable_vis(void) = 0;

	virtual int			build_world_lists_number() const = 0;

	virtual void		set_cheap_water_start_distance(float flCheapWaterStartDistance) = 0;
	virtual void		set_cheap_water_end_distance(float flCheapWaterEndDistance) = 0;

	virtual void		get_water_lod_params(float& flCheapWaterStartDistance, float& flCheapWaterEndDistance) = 0;

	virtual void		drift_pitch(void) = 0;

	virtual void		set_screen_overlay_material(i_material* pMaterial) = 0;
	virtual i_material* get_screen_overlay_material() = 0;

	virtual void		write_save_game_screenshot(const char* pFilename) = 0;
	virtual void		write_save_game_screenshot_of_size(const char* pFilename, int width, int height, bool bCreatePowerOf2Padded = false, bool bWriteVTF = false) = 0;

	virtual void		nothing_01(void) = 0;
	virtual void		nothing_02(void) = 0;

	// Draws another rendering over the top of the screen
	virtual void		queue_overlay_render_view(const c_view_setup& view, int nClearFlags, int whatToDraw) = 0;

	// Returns znear and zfar
	virtual float		get_z_near() = 0;
	virtual float		get_z_far() = 0;

	virtual void		get_screen_fade_distances(float* min, float* max) = 0;

	virtual void* get_currently_drawing_entity() = 0;
	virtual void		set_currently_drawing_entity(void* pEnt) = 0;

	virtual bool		update_shadow_depth_texture(i_texture* pRenderTarget, i_texture* pDepthTexture, const c_view_setup& shadowView) = 0;

	virtual void		freeze_frame(float flFreezeTime) = 0;

	virtual i_replay_screenshot_system* get_replay_screenshot_system() = 0;

	virtual void		nothing_03(void) = 0;
	virtual void		nothing_04(void) = 0;
	virtual void		should_force_no_vis() = 0;									 // dont think the type is right btw 
	virtual void		view_draw_scene_intro(const c_view_setup& view, int nClearFlags, const void* introData) = 0; // dont think the type is right btw 
public:
	//custom
	c_view_setup m_view;
};