#pragma once
#include "../../math/vector.h"
#include <d3d9.h>

enum clear_flags_t
{
	view_clear_color = 0x1,
	view_clear_depth = 0x2,
	view_clear_full_target = 0x4,
	view_no_draw = 0x8,
	view_clear_obey_stencil = 0x10, // Draws a quad allowing stencil test to clear through portals
	view_clear_stencil = 0x20,
};

enum stereo_eye_t
{
	stereo_eye_mono = 0,
	stereo_eye_left = 1,
	stereo_eye_right = 2,
	stereo_eye_max = 3,
};


//-----------------------------------------------------------------------------
// Purpose: Renderer setup data.  
//-----------------------------------------------------------------------------
class c_view_setup
{
public:
	c_view_setup()
	{
		aspect_ratio = 0.0f;
		m_b_render_to_subrect_of_larger_screen = false;
		m_b_do_bloom_and_tone_mapping = true;
		m_b_ortho = false;
		m_b_off_center = false;
		m_b_cache_full_scene_state = false;
		//		m_bUseExplicitViewVector = false;
		m_b_view_to_projection_override = false;
		m_e_stereo_eye = stereo_eye_mono;
	}

	// shared by 2D & 3D views

		// left side of view window
	int			x;
	int			m_n_unscaled_x;
	// top side of view window
	int			y;
	int			m_n_unscaled_y;
	// width of view window
	int			width;
	int			m_n_unscaled_width;
	// height of view window
	int			height;
	// which eye are we rendering?
	stereo_eye_t m_e_stereo_eye;
	int			m_n_unscaled_height;

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_b_ortho;
	// View-space rectangle for ortho projection.
	float		m_ortho_left;
	float		m_ortho_top;
	float		m_ortho_right;
	float		m_ortho_bottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fov_viewmodel;

	// 3D origin of camera
	c_vector		origin;

	// heading of camera (pitch, yaw, roll)
	c_vector		angles;
	// local Z coordinate of near plane of camera
	float		z_near;
	// local Z coordinate of far plane of camera
	float		z_far;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		z_near_viewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		z_far_viewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_b_render_to_subrect_of_larger_screen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		aspect_ratio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_b_off_center;
	float		m_fl_off_center_top;
	float		m_fl_off_center_bottom;
	float		m_fl_off_center_left;
	float		m_fl_off_center_right;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_b_do_bloom_and_tone_mapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_b_cache_full_scene_state;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool        m_b_view_to_projection_override;
	D3DMATRIX     m_view_to_projection;
};