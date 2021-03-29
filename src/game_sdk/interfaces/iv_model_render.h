#pragma once
#include <d3d9.h>

#include "i_material_system.h"
#include "../entitys/i_client_entity.h"

class i_mesh;
class i_pooled_vb_allocator;
struct studiohdr_t;
struct studio_decal_handle_t;
struct studiohwdata_t;
struct light_cache_handle_t;

class c_studio_hdr;



typedef void* model_instance_handle_t;

enum override_type_t
{
	override_normal = 0,
	override_build_shadows,
	override_depth_write,
	override_ssao_depth_write,
};
enum
{
	adddecal_to_all_lods = -1
};
struct static_prop_render_info_t
{
	const D3DMATRIX* p_model_to_world;
	const void* p_model;
	i_client_renderable* p_renderable;
	c_vector* p_lighting_origin;
	short					skin;
	model_instance_handle_t	instance;
};
struct draw_model_state_t
{
	studiohdr_t* m_p_studio_hdr;
	studiohwdata_t* m_p_studio_hw_data;
	i_client_renderable* m_p_renderable;
	const D3DMATRIX* m_p_model_to_world;
	studio_decal_handle_t* m_decals;
	int						m_draw_flags;
	int						m_lod;
};
struct color_mesh_info_t
{
	// A given color mesh can own a unique Mesh, or it can use a shared Mesh
	// (in which case it uses a sub-range defined by m_nVertOffset and m_nNumVerts)
	i_mesh* m_p_mesh;
	i_pooled_vb_allocator* m_p_pooled_vb_allocator;
	int						m_n_vert_offset_in_bytes;
	int						m_n_num_verts;
};
struct draw_model_info_t
{
	studiohdr_t* m_p_studio_hdr;
	studiohwdata_t* m_p_hardware_data;
	studio_decal_handle_t* m_decals;	//MAY BE BUG
	int				m_skin;
	int				m_body;
	int				m_hitbox_set;
	void* m_p_client_entity;
	int				m_lod;
	color_mesh_info_t* m_p_color_meshes;
	bool			m_b_static_lighting;
	c_vector			m_vec_ambient_cube[6];		// ambient, and lights that aren't in locallight[]
	int				m_n_local_light_count;
	light_desc_t		m_local_light_descs[4];
};

struct model_render_info_t
{
	c_vector origin;
	c_vector angles;
	i_client_renderable* renderable;
	const model_t* model;
	const D3DMATRIX* model_to_world;
	const D3DMATRIX* lighting_offset;
	const c_vector* lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	model_instance_handle_t instance;

	model_render_info_t()
	{
		model_to_world = NULL;
		lighting_offset = NULL;
		lighting_origin = NULL;
	}
};

struct ray_t;
class iv_model_render
{
public:
	virtual int		draw_model(int flags,i_client_renderable* p_renderable,model_instance_handle_t instance,int entity_index,const void* model,c_vector const& origin,c_vector const& angles,int skin,int body,int hitboxset,const D3DMATRIX* modelToWorld = NULL,const D3DMATRIX* pLightingOffset = NULL) = 0;
	virtual void	forced_material_override(i_material* newMaterial, override_type_t nOverrideType = override_normal) = 0;
	virtual void	set_view_target(const c_studio_hdr* pStudioHdr, int nBodyIndex, const c_vector& target) = 0;
	virtual model_instance_handle_t create_instance(i_client_renderable* pRenderable, light_cache_handle_t* pCache = NULL) = 0;
	virtual void destroy_instance(model_instance_handle_t handle) = 0;
	virtual void set_static_lighting(model_instance_handle_t handle, light_cache_handle_t* pHandle) = 0;
	virtual light_cache_handle_t get_static_lighting(model_instance_handle_t handle) = 0;
	virtual bool change_instance(model_instance_handle_t handle, i_client_renderable* pRenderable) = 0;
	virtual void add_decal(model_instance_handle_t handle, ray_t const& ray,c_vector const& decalUp, int decalIndex, int body, bool noPokeThru = false, int maxLODToDecal = adddecal_to_all_lods) = 0;
	virtual void gmod_add_decal(model_instance_handle_t handle, c_vector const&, c_vector const&, i_material* Material, int thing, float thing2, float thing3) = 0;
	virtual void remove_all_decals(model_instance_handle_t handle) = 0;
	virtual void remove_all_decals_from_all_models() = 0;
	virtual D3DMATRIX* draw_model_shadow_setup(i_client_renderable* pRenderable, int body, int skin, draw_model_info_t* pInfo, D3DMATRIX* pCustomBoneToWorld = NULL) = 0;
	virtual void draw_model_shadow(i_client_renderable* pRenderable, const draw_model_info_t& info, D3DMATRIX* pCustomBoneToWorld = NULL) = 0;
	virtual bool recompute_static_lighting(model_instance_handle_t handle) = 0;
	virtual void release_all_static_prop_color_data(void) = 0;
	virtual void restore_all_static_prop_color_data(void) = 0;
	virtual int	draw_model_ex(model_render_info_t& pInfo) = 0;
	virtual int	draw_model_ex_static_prop(model_render_info_t& pInfo) = 0;
	virtual bool draw_model_setup(model_render_info_t& pInfo, draw_model_state_t* pState, D3DMATRIX* pCustomBoneToWorld, D3DMATRIX** ppBoneToWorldOut) = 0;
	virtual void draw_model_execute(const draw_model_state_t& state, const model_render_info_t& pInfo, D3DMATRIX* pCustomBoneToWorld = NULL) = 0;
	virtual void setup_lighting(const c_vector& vec_center) = 0;
	virtual int draw_static_prop_array_fast(static_prop_render_info_t* p_props, int count, bool b_shadow_depth) = 0;
	virtual void suppress_engine_lighting(bool b_suppress) = 0;
	virtual void setup_color_meshes(int n_total_verts) = 0;
};