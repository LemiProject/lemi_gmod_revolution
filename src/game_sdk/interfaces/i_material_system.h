#pragma once
#include <string>
#include "../../math/math.h"

#include <memory/virtual_method.h>
#include <d3d9.h>

class i_material_var;
class key_values;
class image_format;
class i_material;

using preview_image_ret_val_t = int;
using vertex_format_t = int;
using morph_format_t = int;
using material_property_types_t = int;

enum material_var_flags_t
{
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	material_var_multipass = (1 << 9),
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17),
	material_var_noalphamod = (1 << 18),
	material_var_envmapcameraspace = (1 << 19),
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23),
	material_var_opaquetexture = (1 << 24),
	material_var_envmapmode = (1 << 25),
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
	material_var_ignore_alpha_modulation = (1 << 30),
};


class i_material
{
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_texture_group_name() const = 0;
	virtual preview_image_ret_val_t get_preview_image_properties(int* width, int* height,
		image_format* image_format, bool* is_translucent) const = 0;
	virtual preview_image_ret_val_t get_preview_image(unsigned char* data,
		int width, int height,
		image_format imageFormat) const = 0;
	virtual int				get_mapping_width() = 0;
	virtual int				get_mapping_height() = 0;
	virtual int				get_num_animation_frames() = 0;
	virtual bool			in_material_page(void) = 0;
	virtual	void			get_material_offset(float* pOffset) = 0;
	virtual void			get_material_scale(float* pScale) = 0;
	virtual i_material* get_material_page(void) = 0;
	virtual i_material_var* find_var(const char* varName, bool* found, bool complain = true) = 0;
	virtual void			increment_reference_count(void) = 0;
	virtual void			decrement_reference_count(void) = 0;
	inline void add_ref() { increment_reference_count(); }
	inline void release() { decrement_reference_count(); }
	virtual int 			get_enumeration_id(void) const = 0;
	virtual void			get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void			recompute_state_snapshots() = 0;
	virtual bool			is_translucent() = 0;
	virtual bool			is_alpha_tested() = 0;
	virtual bool			is_vertex_lit() = 0;
	virtual vertex_format_t	get_vertex_format() const = 0;
	virtual bool			has_proxy(void) const = 0;
	virtual bool			uses_env_cubemap(void) = 0;
	virtual bool			needs_tangent_space(void) = 0;
	virtual bool			needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			needs_software_skinning(void) = 0;
	virtual void			alpha_modulate(float alpha) = 0;
	virtual void			color_modulate(float r, float g, float b) = 0;
	virtual void			set_material_var_flag(material_var_flags_t flag, bool on) = 0;
	virtual bool			get_material_var_flag(material_var_flags_t flag) const = 0;
	virtual void			get_reflectivity(c_vector& reflect) = 0;
	virtual bool			get_property_flag(material_property_types_t type) = 0;
	virtual bool			is_two_sided() = 0;
	virtual void			set_shader(const char* pShaderName) = 0;
	virtual int				get_num_passes(void) = 0;
	virtual int				get_texture_memory_bytes(void) = 0;
	virtual void			refresh() = 0;
	virtual bool			needs_lightmap_blend_alpha(void) = 0;
	virtual bool			needs_software_lighting(void) = 0;
	virtual int				shader_param_count() const = 0;
	virtual i_material_var** get_shader_params(void) = 0;
	virtual bool			is_error_material() const = 0;
	virtual void			set_use_fixed_function_baked_lighting(bool bEnable) = 0;
	virtual float			get_alpha_modulation() = 0;
	virtual void			get_color_modulation(float* r, float* g, float* b) = 0;
	virtual morph_format_t	get_morph_format() const = 0;
	virtual i_material_var* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void			set_shader_and_params(key_values* pKeyValues) = 0;
	virtual const char* get_shader_name() const = 0;
	virtual void			delete_if_unreferenced() = 0;
	virtual bool			is_sprite_card() = 0;
	virtual void			call_bind_proxy(void* proxyData) = 0;
	virtual i_material* check_proxy_replacement(void* proxyData) = 0;
	virtual void			refresh_preserving_material_vars() = 0;
	virtual bool			was_reloaded_from_whitelist() = 0;
	virtual bool	is_precached() const = 0;
};

class i_texture;
class i_mesh;
class i_vertex_buffer;
class i_index_buffer;
class i_morph;
class rect_t;
class i_call_queue;

using light_desc_t = void*;

using material_matrix_mode_t = int;
using material_cull_mode_t = int;
using material_fog_mode_t = int;
using material_height_clip_mode_t = int;
using material_index_format_t = int;
using material_primitive_type_t = int;
using occlusion_query_object_handle_t = int;
using flashlight_state_t = int;
using morph_weight_t = int;
using stencil_operation_t = int;
using stencil_comparison_function_t = int;
using deformation_base_t = int;
using color_correction_handle_t = int;
using material_non_interactive_mode_t = int;

class i_mat_render_context
{
public:
	/*0*/   virtual void	begin_render() = 0;
	/*1*/	virtual void	end_render() = 0;
	/*2*/	virtual void	flush(bool flushHardware = false) = 0;
	/*3*/	virtual void	bind_local_cubemap(i_texture* pTexture) = 0;
	/*4*/	virtual void	set_render_target(i_texture* pTexture) = 0;
	/*5*/	virtual i_texture* get_render_target(void) = 0;
	/*6*/	virtual void	get_render_target_dimensions(int& width, int& height) const = 0;
	/*7*/	virtual void	bind(i_material* material, void* proxyData = 0) = 0;
	/*8*/	virtual void	bind_lightmap_page(int lightmapPageID) = 0;
	/*9*/	virtual void	depth_range(float zNear, float zFar) = 0;
	/*10*/	virtual void	clear_buffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;
	/*11*/	virtual void	read_pixels(int x, int y, int width, int height, unsigned char* data, image_format dstFormat) = 0;
	/*12*/	virtual void	set_ambient_light(float r, float g, float b) = 0;
	/*13*/	virtual void	set_light(int lightNum, const light_desc_t& desc) = 0;
	/*14*/	virtual void	set_ambient_light_cube(void* cube) = 0;
	/*15*/	virtual void	copy_render_target_to_texture(i_texture* pTexture) = 0;
	/*16*/	virtual void	set_frame_buffer_copy_texture(i_texture* pTexture, int textureIndex = 0) = 0;
	/*17*/	virtual i_texture* get_frame_buffer_copy_texture(int textureIndex) = 0;
	/*18*/	virtual void	matrix_mode(material_matrix_mode_t matrixMode) = 0;
	/*19*/	virtual void	push_matrix(void) = 0;
	/*20*/	virtual void	pop_matrix(void) = 0;
	/*21*/	virtual void	load_matrix(D3DMATRIX const& matrix) = 0;
	/*22*/	virtual void	load_matrix(void* const& matrix) = 0;
	/*23*/	virtual void	mult_matrix(D3DMATRIX const& matrix) = 0;
	/*24*/	virtual void	mult_matrix(void* const& matrix) = 0;
	/*25*/	virtual void	mult_matrix_local(D3DMATRIX const& matrix) = 0;
	/*26*/	virtual void	mult_matrix_local(void* const& matrix) = 0;
	/*27*/	virtual void	get_matrix(material_matrix_mode_t matrixMode, D3DMATRIX* matrix) = 0;
	/*28*/	virtual void	get_matrix(material_matrix_mode_t matrixMode, void** matrix) = 0;
	/*29*/	virtual void	load_identity(void) = 0;
	/*30*/	virtual void	ortho(double left, double top, double right, double bottom, double zNear, double zFar) = 0;
	/*31*/	virtual void	perspective_x(double fovx, double aspect, double zNear, double zFar) = 0;
	/*32*/	virtual void	pick_matrix(int x, int y, int width, int height) = 0;
	/*33*/	virtual void	rotate(float angle, float x, float y, float z) = 0;
	/*34*/	virtual void	translate(float x, float y, float z) = 0;
	/*35*/	virtual void	scale(float x, float y, float z) = 0;
	/*36*/	virtual void	viewport(int x, int y, int width, int height) = 0;
	/*37*/	virtual void	get_viewport(int& x, int& y, int& width, int& height) const = 0;
	/*38*/	virtual void	cull_mode(material_cull_mode_t cullMode) = 0;
	/*39*/	virtual void	set_height_clip_mode(material_height_clip_mode_t nHeightClipMode) = 0;
	/*40*/	virtual void	set_height_clip_z(float z) = 0;
	/*41*/	virtual void	fog_mode(material_fog_mode_t fogMode) = 0;
	/*42*/	virtual void	fog_start(float fStart) = 0;
	/*43*/	virtual void	fog_end(float fEnd) = 0;
	/*44*/	virtual void	set_fog_z(float fogZ) = 0;
	/*45*/	virtual material_fog_mode_t	get_fog_mode(void) = 0;
	/*46*/	virtual void	fog_color3_f(float r, float g, float b) = 0;
	/*47*/	virtual void	fog_color3_fv(float const* rgb) = 0;
	/*48*/	virtual void	fog_color3_ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	/*49*/	virtual void	fog_color3_ubv(unsigned char const* rgb) = 0;
	/*50*/	virtual void	get_fog_color(unsigned char* rgb) = 0;
	/*51*/	virtual void	set_num_bone_weights(int numBones) = 0;
	/*52*/	virtual i_mesh* create_static_mesh(vertex_format_t fmt, const char* pTextureBudgetGroup, i_material* pMaterial = NULL) = 0;
	/*53*/	virtual void	destroy_static_mesh(i_mesh* mesh) = 0;
	/*54*/	virtual i_mesh* get_dynamic_mesh(bool buffered = true, i_mesh* pVertexOverride = 0, i_mesh* pIndexOverride = 0, i_material* pAutoBind = 0) = 0;
	/*55*/	virtual i_vertex_buffer* create_static_vertex_buffer(vertex_format_t fmt, int nVertexCount, const char* pTextureBudgetGroup) = 0;
	/*56*/	virtual i_index_buffer* create_static_index_buffer(material_index_format_t fmt, int nIndexCount, const char* pTextureBudgetGroup) = 0;
	/*57*/	virtual void	destroy_vertex_buffer(i_vertex_buffer*) = 0;
	/*58*/	virtual void	destroy_index_buffer(i_index_buffer*) = 0;
	/*59*/	virtual i_vertex_buffer* get_dynamic_vertex_buffer(int streamID, vertex_format_t vertexFormat, bool bBuffered = true) = 0;
	/*60*/	virtual i_index_buffer* get_dynamic_index_buffer(material_index_format_t fmt, bool bBuffered = true) = 0;
	/*61*/	virtual void	bind_vertex_buffer(int streamID, i_vertex_buffer* pVertexBuffer, int nOffsetInBytes, int nFirstVertex, int nVertexCount, vertex_format_t fmt, int nRepetitions = 1) = 0;
	/*62*/	virtual void	bind_index_buffer(i_index_buffer* pIndexBuffer, int nOffsetInBytes) = 0;
	/*63*/	virtual void	draw(material_primitive_type_t primitiveType, int firstIndex, int numIndices) = 0;
	/*64*/	virtual int		selection_mode(bool selectionMode) = 0;
	/*65*/	virtual void	selection_buffer(unsigned int* pBuffer, int size) = 0;
	/*66*/	virtual void	clear_selection_names() = 0;
	/*67*/	virtual void	load_selection_name(int name) = 0;
	/*68*/	virtual void	push_selection_name(int name) = 0;
	/*69*/	virtual void	pop_selection_name() = 0;
	/*70*/	virtual void	clear_color3_ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	/*71*/	virtual void	clear_color4_ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
	/*72*/	virtual void	override_depth_enable(bool bEnable, bool bDepthEnable) = 0;
	/*73*/	virtual void	draw_screen_space_quad(i_material* pMaterial) = 0;
	/*74*/	virtual void	sync_token(const char* pToken) = 0;
	/*75*/	virtual float	compute_pixel_width_of_sphere(const c_vector& origin, float flRadius) = 0;
	/*76*/	virtual occlusion_query_object_handle_t create_occlusion_query_object(void) = 0;
	/*77*/	virtual void	destroy_occlusion_query_object(occlusion_query_object_handle_t) = 0;
	/*78*/	virtual void	begin_occlusion_query_drawing(occlusion_query_object_handle_t) = 0;
	/*79*/	virtual void	end_occlusion_query_drawing(occlusion_query_object_handle_t) = 0;
	/*80*/	virtual int		occlusion_query_get_num_pixels_rendered(occlusion_query_object_handle_t) = 0;
	/*81*/	virtual void	set_flashlight_mode(bool bEnable) = 0;
	/*82*/	virtual void	set_flashlight_state(const flashlight_state_t& state, const D3DMATRIX& worldToTexture) = 0;
	/*83*/	virtual material_height_clip_mode_t get_height_clip_mode() = 0;
	/*84*/	virtual float	compute_pixel_diameter_of_sphere(const c_vector& vecAbsOrigin, float flRadius) = 0;
	/*85*/	virtual void	enable_user_clip_transform_override(bool bEnable) = 0;
	/*86*/	virtual void	user_clip_transform(const D3DMATRIX& worldToView) = 0;
	/*87*/	virtual bool	get_flashlight_mode() const = 0;
	/*88*/	virtual void	reset_occlusion_query_object(occlusion_query_object_handle_t) = 0;
	/*89*/	virtual void	unused3() = 0;
	/*90*/	virtual i_morph* create_morph(morph_format_t format, const char* pDebugName) = 0;
	/*91*/	virtual void	destroy_morph(i_morph* pMorph) = 0;
	/*92*/	virtual void	bind_morph(i_morph* pMorph) = 0;
	/*93*/	virtual void	set_flex_weights(int nFirstWeight, int nCount, const morph_weight_t* pWeights) = 0;
	/*94*/	virtual void	unused4() = 0;
	/*95*/	virtual void	unused5() = 0;
	/*96*/	virtual void	unused6() = 0;
	/*97*/	virtual void	unused7() = 0;
	/*98*/	virtual void	unused8() = 0;
	/*99*/	virtual void	read_pixels_and_stretch(rect_t* pSrcRect, rect_t* pDstRect, unsigned char* pBuffer, image_format dstFormat, int nDstStride) = 0;
	/*100*/	virtual void	get_window_size(int& width, int& height) const = 0;
	/*101*/	virtual void	draw_screen_space_rectangle(i_material* pMaterial, int destx, int desty, int width, int height, float src_texture_x0, float src_texture_y0, float src_texture_x1, float src_texture_y1, int src_texture_width, int src_texture_height, int nXDice = 1, int nYDice = 1) = 0;
	/*102*/	virtual void	load_bone_matrix(int boneIndex, const D3DMATRIX& matrix) = 0;
	/*103*/	virtual void	push_render_target_and_viewport() = 0;
	/*104*/	virtual void	push_render_target_and_viewport(i_texture* pTexture) = 0;
	/*105*/	virtual void	push_render_target_and_viewport(i_texture* pTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;
	/*106*/	virtual void	push_render_target_and_viewport(i_texture* pTexture, i_texture* pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;
	/*107*/	virtual void	pop_render_target_and_viewport(void) = 0;
	/*108*/	virtual void	bind_lightmap_texture(i_texture* pLightmapTexture) = 0;
	/*109*/	virtual void	copy_render_target_to_texture_ex(i_texture* pTexture, int nRenderTargetID, rect_t* pSrcRect, rect_t* pDstRect = NULL) = 0;
	/*110*/	virtual void	copy_texture_to_render_target_ex(int nRenderTargetID, i_texture* pTexture, rect_t* pSrcRect, rect_t* pDstRect = NULL) = 0;
	/*111*/	virtual void	perspective_off_center_x(double fovx, double aspect, double zNear, double zFar, double bottom, double top, double left, double right) = 0;
	/*112*/	virtual void	set_float_rendering_parameter(int parm_number, float value) = 0;
	/*113*/	virtual void	set_int_rendering_parameter(int parm_number, int value) = 0;
	/*114*/	virtual void	set_vector_rendering_parameter(int parm_number, c_vector const& value) = 0;
	/*115*/	virtual void	set_stencil_enable(bool onoff) = 0;
	/*116*/	virtual void	set_stencil_fail_operation(stencil_operation_t op) = 0;
	/*117*/	virtual void	set_stencil_z_fail_operation(stencil_operation_t op) = 0;
	/*118*/	virtual void	set_stencil_pass_operation(stencil_operation_t op) = 0;
	/*119*/	virtual void	set_stencil_compare_function(stencil_comparison_function_t cmpfn) = 0;
	/*120*/	virtual void	set_stencil_reference_value(int ref) = 0;
	/*121*/	virtual void	set_stencil_test_mask(UINT32 msk) = 0;
	/*122*/	virtual void	set_stencil_write_mask(UINT32 msk) = 0;
	/*123*/	virtual void	clear_stencil_buffer_rectangle(int xmin, int ymin, int xmax, int ymax, int value) = 0;
	/*124*/	virtual void	set_render_target_ex(int nRenderTargetID, i_texture* pTexture) = 0;
	/*125*/	virtual void	push_custom_clip_plane(const float* pPlane) = 0;
	/*126*/	virtual void	pop_custom_clip_plane(void) = 0;
	/*127*/	virtual void	get_max_to_render(i_mesh* pMesh, bool bMaxUntilFlush, int* pMaxVerts, int* pMaxIndices) = 0;
	/*128*/	virtual int		get_max_vertices_to_render(i_material* pMaterial) = 0;
	/*129*/	virtual int		get_max_indices_to_render() = 0;
	/*130*/	virtual void	disable_all_local_lights() = 0;
	/*131*/	virtual int		compare_material_combos(i_material* pMaterial1, i_material* pMaterial2, int lightMapID1, int lightMapID2) = 0;
	/*132*/	virtual i_mesh* get_flex_mesh() = 0;
	/*133*/	virtual void	set_flashlight_state_ex(const flashlight_state_t& state, const D3DMATRIX& worldToTexture, i_texture* pFlashlightDepthTexture) = 0;
	/*134*/	virtual i_texture* get_local_cubemap() = 0;
	/*135*/	virtual void	clear_buffers_obey_stencil(bool bClearColor, bool bClearDepth) = 0;
	/*136*/	virtual bool	enable_clipping(bool bEnable) = 0;
	/*137*/	virtual void	get_fog_distances(float* fStart, float* fEnd, float* fFogZ) = 0;
	/*138*/	virtual void	begin_pix_event(unsigned long color, const char* szName) = 0;
	/*139*/	virtual void	end_pix_event() = 0;
	/*140*/	virtual void	set_pix_marker(unsigned long color, const char* szName) = 0;
	/*141*/	virtual void	begin_batch(i_mesh* pIndices) = 0;
	/*142*/	virtual void	bind_batch(i_mesh* pVertices, i_material* pAutoBind = NULL) = 0;
	/*143*/	virtual void	draw_batch(int firstIndex, int numIndices) = 0;
	/*144*/	virtual void	end_batch() = 0;
	/*145*/	virtual i_call_queue* get_call_queue() = 0;
	/*146*/	virtual void	get_world_space_camera_position(c_vector* pCameraPos) = 0;
	/*147*/	virtual void	get_world_space_camera_vectors(c_vector* pVecForward, c_vector* pVecRight, c_vector* pVecUp) = 0;
	/*148*/	virtual void	reset_tone_mapping_scale(float monoscale) = 0; 			// set scale to monoscale instantly with no chasing
	/*149*/	virtual void	set_goal_tone_mapping_scale(float monoscale) = 0;
	/*150*/	virtual void	turn_on_tone_mapping() = 0;
	/*151*/	virtual void	set_tone_mapping_scale_linear(const c_vector& scale) = 0;
	/*152*/	virtual c_vector	get_tone_mapping_scale_linear(void) = 0;
	/*153*/	virtual void	set_shadow_depth_bias_factors(float fSlopeScaleDepthBias, float fDepthBias) = 0;
	/*154*/	virtual void	perform_full_screen_stencil_operation(void) = 0;
	/*155*/	virtual void	set_lighting_origin(c_vector vLightingOrigin) = 0;
	/*156*/	virtual void	set_scissor_rect(const int nLeft, const int nTop, const int nRight, const int nBottom, const bool bEnableScissor) = 0;
	/*157*/	virtual void	begin_morph_accumulation() = 0;
	/*158*/	virtual void	end_morph_accumulation() = 0;
	/*159*/	virtual void	accumulate_morph(i_morph* pMorph, int nMorphCount, const morph_weight_t* pWeights) = 0;
	/*160*/	virtual void	push_deformation(deformation_base_t const* Deformation) = 0;
	/*161*/	virtual void	pop_deformation() = 0;
	/*162*/	virtual int		get_num_active_deformations() const = 0;
	/*163*/	virtual bool	get_morph_accumulator_tex_coord(c_vector* pTexCoord, i_morph* pMorph, int nVertex) = 0;
	/*164*/	virtual i_mesh* get_dynamic_mesh_ex(vertex_format_t vertexFormat, bool bBuffered = true, i_mesh* pVertexOverride = 0, i_mesh* pIndexOverride = 0, i_material* pAutoBind = 0) = 0;
	/*165*/	virtual void	fog_max_density(float flMaxDensity) = 0;
	/*166*/	virtual i_material* get_current_material() = 0;
	/*167*/	virtual int		get_current_num_bones() const = 0;
	/*168*/	virtual void* get_current_proxy() = 0;
	/*169*/	virtual void	enable_color_correction(bool bEnable) = 0;
	/*170*/	virtual color_correction_handle_t add_lookup(const char* pName) = 0;
	/*171*/	virtual bool	remove_lookup(color_correction_handle_t handle) = 0;
	/*172*/	virtual void	lock_lookup(color_correction_handle_t handle) = 0;
	/*173*/	virtual void	load_lookup(color_correction_handle_t handle, const char* pLookupName) = 0;
	/*174*/	virtual void	unlock_lookup(color_correction_handle_t handle) = 0;
	/*175*/	virtual void	set_lookup_weight(color_correction_handle_t handle, float flWeight) = 0;
	/*176*/	virtual void	reset_lookup_weights() = 0;
	/*177*/	virtual void	set_resetable(color_correction_handle_t handle, bool bResetable) = 0;
	/*178*/	virtual void	set_full_screen_depth_texture_validity_flag(bool bIsValid) = 0;
	/*179*/	virtual void	set_non_interactive_pacifier_texture(i_texture* pTexture, float flNormalizedX, float flNormalizedY, float flNormalizedSize) = 0;
	/*180*/	virtual void	set_non_interactive_temp_fullscreen_buffer(i_texture* pTexture, material_non_interactive_mode_t mode) = 0;
	/*181*/	virtual void	enable_non_interactive_mode(material_non_interactive_mode_t mode) = 0;
	/*182*/	virtual void	refresh_front_buffer_non_interactive() = 0;
	/*183*/	virtual void* lock_render_data(int nSizeInBytes) = 0;
	/*184*/	virtual void	unlock_render_data(void* pData) = 0;
	/*185*/	virtual void	add_ref_render_data() = 0;
	/*186*/	virtual void	release_render_data() = 0;
	/*187*/	virtual bool	is_render_data(const void* pData) const = 0;
	/*188*/	virtual void	printf_va(char* fmt, va_list vargs) = 0;
	/*189*/	virtual void	printf(const char* fmt, ...) = 0;
	/*190*/	virtual float	knob(char* knobname, float* setvalue = NULL) = 0;
	/*191*/	virtual void	override_alpha_write_enable(bool bEnable, bool bAlphaWriteEnable) = 0;
	/*192*/	virtual void	override_color_write_enable(bool bOverrideEnable, bool bColorWriteEnable) = 0;
	/*193*/	virtual void	clear_buffers_obey_stencil_ex(bool bClearColor, bool bClearAlpha, bool bClearDepth) = 0;
};

class i_material_system
{
public:
	i_material* create_material(const char* material_name, void* vmt_key_values)
	{
		using fn = i_material * (__thiscall*)(void*, const char*, void*);
		return (*(fn**)this)[70](this, material_name, vmt_key_values);
	}

	i_material* find_material(const char* material_name, const char* group_name, bool complain = true, const char* complain_prefix = nullptr)
	{
		using fn = i_material * (__thiscall*)(void*, const char*, const char*, bool, const char*);
		return (*(fn**)this)[71](this, material_name, group_name, complain, complain_prefix);
	}

	//i_mat_render_context* get_render_context()
	//{
	//	using fn = i_mat_render_context * (__thiscall*)(void*);
	//	return (*(fn**)this)[98](this);
	//}

	VIRTUAL_METHOD(i_mat_render_context*, get_render_context, 102, (), (this));
	
	i_mat_render_context* create_render_context(void* material_context_type)
	{
		using fn = i_mat_render_context * (__thiscall*)(void*, void*);
		return (*(fn**)this)[106](this, material_context_type);
	}

	i_mat_render_context* set_render_context(i_mat_render_context* new_render)
	{
		using fn = i_mat_render_context * (__thiscall*)(void*, void*);
		return (*(fn**)this)[107](this, new_render);
	}
};

class ivtf_texture;

class i_texture_regenerator
{
public:
	virtual void regenerate_texture_bits(i_texture * pTexture, ivtf_texture * pVTFTexture, rect_t * pRect) = 0;
	virtual void release() = 0;
};

class i_texture
{
public:
	virtual const char* get_name(void) const = 0;
	virtual int get_mapping_width() const = 0;
	virtual int get_mapping_height() const = 0;
	virtual int get_actual_width() const = 0;
	virtual int get_actual_height() const = 0;
	virtual int get_num_animation_frames() const = 0;
	virtual bool is_translucent() const = 0;
	virtual bool is_mipmapped() const = 0;
	virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void* get_resource_data(uint32_t eDataType, size_t* pNumBytes) const = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	inline void add_ref() { increment_reference_count(); }
	inline void release() { decrement_reference_count(); }
	virtual void set_texture_regenerator(i_texture_regenerator* pTextureRegen) = 0;
	virtual void download(rect_t* pRect = 0, int nAdditionalCreationFlags = 0) = 0;
	virtual int get_approximate_vid_mem_bytes(void) const = 0;
	virtual bool is_error() const = 0;
	virtual bool is_volume_texture() const = 0;
	virtual int get_mapping_depth() const = 0;
	virtual int get_actual_depth() const = 0;
	virtual image_format get_image_format() const = 0;
	virtual int get_normal_decode_mode() const = 0;
	virtual bool is_render_target() const = 0;
	virtual bool is_cube_map() const = 0;
	virtual bool is_normal_map() const = 0;
	virtual bool is_procedural() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual void swap_contents(i_texture* pOther) = 0;
	virtual unsigned int get_flags(void) const = 0;
	virtual void force_lod_override(int iNumLodsOverrideUpOrDown) = 0;
	virtual bool save_to_file(const char* fileName) = 0;
};