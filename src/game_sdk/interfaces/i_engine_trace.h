#pragma once

#pragma once

#define  Assert( _exp ) ((void)0)


#pragma region MASKS

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40 
#define   CONTENTS_OPAQUE               0x80 
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200     
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800 
#define   CONTENTS_TEAM2                0x1000 
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000 

#define   CONTENTS_MONSTER              0x2000000 
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000 
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001 
#define   SURF_SKY2D                    0x0002 
#define   SURF_SKY                      0x0004 
#define   SURF_WARP                     0x0008 
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020 
#define   SURF_TRIGGER                  0x0040 
#define   SURF_NODRAW                   0x0080 

#define   SURF_HINT                     0x0100 

#define   SURF_SKIP                     0x0200   
#define   SURF_NOLIGHT                  0x0400   
#define   SURF_BUMPLIGHT                0x0800   
#define   SURF_NOSHADOWS                0x1000   
#define   SURF_NODECALS                 0x2000   
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000   
#define   SURF_HITBOX                   0x8000   

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)
#pragma endregion


#include "../../math/math.h"

struct ray_t
{
	c_vector_aligned  m_start;    // starting point, centered within the extents
	c_vector_aligned  m_delta;    // direction + length of the ray
	c_vector_aligned  m_start_offset;    // Add this to m_Start to get the actual ray start
	c_vector_aligned  m_extents;    // Describes an axis aligned box extruded along a ray
	//const matrix3x4_t *m_pWorldAxisTransform;
	bool    m_is_ray;    // are the extents zero?
	bool    m_is_swept;    // is delta != 0?

	void init(c_vector& vecStart, c_vector& vecEnd)
	{
		m_delta = vecEnd - vecStart;

		m_is_swept = (m_delta.length() != 0);

		m_extents[0] = m_extents[1] = m_extents[2] = 0.0f;
		m_is_ray = true;

		m_start_offset[0] = m_start_offset[1] = m_start_offset[2] = 0.0f;

		m_start = vecStart;
	}
private:
};


struct cplane_t
{
	c_vector	normal;
	float	dist;
	unsigned char type;			// for fast side tests
	unsigned char signbits;		// signx + (signy<<1) + (signz<<1)
	unsigned char pad[2];

	cplane_t() {}

private:
	// No copy constructors allowed if we're in optimal mode
	cplane_t(const cplane_t& vOther);
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

struct trace_t
{
	c_vector startpos;
	c_vector endpos;
	cplane_t	plane;
	float fraction;
	int contents;
	unsigned short disp_flags;
	bool allsolid;
	bool startsolid;
	float fractionleftsolid;
	csurface_t	surface;
	int hitgroup;
	short physicsbone;
	c_base_entity* m_pEnt;
	int hitbox;
};

enum trace_type_t
{
	trace_everything = 0,
	trace_world_only,				// NOTE: This does *not* test static props!!!
	trace_entities_only,			// NOTE: This version will *not* test static props
	trace_everything_filter_props,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class i_trace_filter
{
public:
	virtual ~i_trace_filter() = default;
	virtual bool			should_hit_entity(void* pEntity, int mask) = 0;
	virtual trace_type_t            get_trace_type() const = 0;
};

class c_trace_filter : public i_trace_filter
{
public:
	bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual trace_type_t	get_trace_type() const
	{
		return trace_everything;
	}
	void* pSkip;
};

typedef bool(*should_hit_func_t)(void* pHandleEntity, int contentsMask);
class CTraceFilterSimple : public c_trace_filter
{
public:
	// It does have a base, but we'll never network anything below here..

	CTraceFilterSimple(const void* passentity, int collisionGroup, should_hit_func_t pExtraShouldHitCheckFn = NULL);
	virtual bool should_hit_entity(void* pHandleEntity, int contentsMask);
	virtual void set_pass_entity(const void* pPassEntity) { m_p_pass_ent_ = pPassEntity; }
	virtual void set_collision_group(int iCollisionGroup) { m_collision_group_ = iCollisionGroup; }

	const void* get_pass_entity(void) { return m_p_pass_ent_; }

private:
	const void* m_p_pass_ent_;
	int m_collision_group_;
	should_hit_func_t m_p_extra_should_hit_check_function_;

};

class i_handle_entity;
class c_trace_list_data;
class c_phys_collide;

class i_entity_enumerator
{
public:
	// This gets called with each handle
	virtual bool enum_entity(i_handle_entity* pHandleEntity) = 0;
};

class i_engine_trace
{
public:
	virtual void unused_0() = 0;
	virtual void unused_1() = 0;
	virtual void unused_2() = 0;
	virtual void unused_3() = 0;
	virtual	void trace_ray(const ray_t& ray, unsigned int fMask, i_trace_filter* pTraceFilter, trace_t* pTrace) = 0;
};