#pragma once
#include <cstddef>

enum class e_type
{
	client,
	server,
	menu
};

enum class e_special
{
	glob = 0,
	env,
	reg
};

enum class e_lua_type
{
	type_invalid = -1,
	type_nil,
	type_bool,
	type_lightuserdata,
	type_number,
	type_string,
	type_table,
	type_function,
	type_userdata,
	type_thread,

	type_entity,
	type_vector,
	type_angle,
	type_physobj,
	type_save,
	type_restore,
	type_damageinfo,
	type_effectdata,
	type_movedata,
	type_recipientfilter,
	type_usercmd,
	type_scriptedvehicle,

	type_material,
	type_panel,
	type_particle,
	type_particleemitter,
	type_texture,
	type_usermsg,

	type_convar,
	type_imesh,
	type_matrix,
	type_sound,
	type_pixelvishandle,
	type_dlight,
	type_video,
	type_file,

	type_player = type_entity,
};


class _i_lua_object
{
public:
	virtual ~_i_lua_object() = default;
	virtual void			set(_i_lua_object* obj) = 0;
	virtual void			set_from_stack(int i) = 0;
	virtual void			un_reference() = 0;

	virtual int				get_type(void) = 0;

	virtual const char* get_string(void) = 0;
	virtual float			get_float(void) = 0;
	virtual int				get_int(void) = 0;

	virtual void* get_user_data(void) = 0;

	virtual void			set_member(const char* name) = 0;
	virtual void			set_member(const char* name, _i_lua_object* obj) = 0; // ( This is also used to set nil by passing NULL )
	virtual void			set_member(const char* name, float val) = 0;
	virtual void			set_member(const char* name, bool val) = 0;
	virtual void			set_member(const char* name, const char* val) = 0;
	virtual void			set_member(const char* name, void* f) = 0;

	virtual bool			get_member_bool(const char* name, bool b = true) = 0;
	virtual int				get_member_int(const char* name, int i = 0) = 0;
	virtual float			get_member_float(const char* name, float f = 0.0f) = 0;
	virtual const char* get_member_str(const char* name, const char* = "") = 0;
	virtual void* get_member_user_data(const char* name, void* = 0) = 0;
	virtual void* get_member_user_data(float name, void* = 0) = 0;
	virtual _i_lua_object* get_member(const char* name) = 0;
	virtual _i_lua_object* get_member(_i_lua_object*) = 0;

	virtual void			set_meta_table(_i_lua_object* obj) = 0;
	virtual void			set_user_data(void* obj) = 0;

	virtual void			push(void) = 0;

	virtual bool			is_nil() = 0;
	virtual bool			is_table() = 0;
	virtual bool			is_string() = 0;
	virtual bool			is_number() = 0;
	virtual bool			is_function() = 0;
	virtual bool			is_user_data() = 0;

	virtual _i_lua_object* get_member(float fKey) = 0;

	virtual void* remove_me_1(const char* name, void* = 0) = 0;

	virtual void			set_member(float fKey) = 0;
	virtual void			set_member(float fKey, _i_lua_object* obj) = 0;
	virtual void			set_member(float fKey, float val) = 0;
	virtual void			set_member(float fKey, bool val) = 0;
	virtual void			set_member(float fKey, const char* val) = 0;
	virtual void			set_member(float fKey, void* f) = 0;

	virtual const char* get_member_str(float name, const char* = "") = 0;

	virtual void			set_member(_i_lua_object* k, _i_lua_object* v) = 0;
	virtual bool			get_bool(void) = 0;

	// Push members to table from stack
	virtual bool			push_member_fast(int iStackPos) = 0;
	virtual void			set_member_fast(int iKey, int iValue) = 0;

	virtual void			set_float(float val) = 0;
	virtual void			set_string(const char* val) = 0;
	virtual double			get_double(void) = 0;

	virtual void			set_member_fix_key(char  const*, float) = 0;
	virtual void			set_member_fix_key(char  const*, char  const*) = 0;
	virtual void			set_member_fix_key(char  const*, _i_lua_object*) = 0;

	virtual bool			is_bool(void) = 0;
	virtual void			set_member_double(char  const*, double) = 0;
	virtual void			set_member_nil(char  const*) = 0;
	virtual void			set_member_nil(float) = 0;

	virtual bool			debug_is_unreferenced() = 0;

	virtual void			init(void) = 0;
	virtual void			set_from_global(char  const*) = 0;

	virtual void			set_member(char  const*, unsigned long long) = 0;
	virtual void			set_reference(int i) = 0;

	virtual void			remove_member(char  const*) = 0;
	virtual void			remove_member(float) = 0;
	virtual bool			member_is_nil(char  const*) = 0;

	virtual void			set_member_double(float, double) = 0;
	virtual double			get_member_double(char  const*, double) = 0;
};

class i_lua_object : public _i_lua_object
{
public:
	virtual bool			push_member_fast(int iStackPos) = 0;
	virtual void			set_member_fast(int iKey, int iValue) = 0;

	virtual void			set_float(float val) = 0;
	virtual void			set_string(const char* val) = 0;

	virtual void* get_members(void) = 0;

	// Set member 'pointer'. No GC, no metatables. 
	virtual void			set_member_user_data_lite(const char* strKeyName, void* pData) = 0;
	virtual void* get_member_user_data_lite(const char* strKeyName) = 0;
};

class c_lua_interface;

class c_lua_object : public i_lua_object
{
public:
	int						_unk1;				//0004
	int						_unk2;				//0008
	int						_unk3;				//000C
	c_lua_interface* lua;				//0010
};

using number_t = double;

class c_lua_interface
{
public:
	virtual int			top(void) = 0;
	virtual void		push(int iStackPos) = 0;
	virtual void		pop(int iAmt = 1) = 0;
	virtual void		get_table(int iStackPos) = 0;
	virtual void		get_field(int iStackPos, const char* strName) = 0;
	virtual void		set_field(int iStackPos, const char* strName) = 0;
	virtual void		create_table() = 0;
	virtual void		set_table(int i) = 0;
	virtual void		set_meta_table(int i) = 0;
	virtual bool		get_meta_table(int i) = 0;
	virtual void		call(int iArgs, int iResults) = 0;
	virtual int			p_call(int iArgs, int iResults, int iErrorFunc) = 0;
	virtual int			equal(int iA, int iB) = 0;
	virtual int			raw_equal(int iA, int iB) = 0;
	virtual void		insert(int iStackPos) = 0;
	virtual void		remove(int iStackPos) = 0;
	virtual int			next(int iStackPos) = 0;
	virtual void* new_userdata(unsigned int iSize) = 0;
	virtual void		throw_error(const char* strError) = 0;
	virtual void		check_type(int iStackPos, int iType) = 0;
	virtual void		arg_error(int iArgNum, const char* strMessage) = 0;
	virtual void		raw_get(int iStackPos) = 0;
	virtual void		raw_set(int iStackPos) = 0;
	virtual const char* get_string(int iStackPos = -1, unsigned int* iOutLen = NULL) = 0;
	virtual double			get_number(int iStackPos = -1) = 0;
	virtual bool			get_bool(int iStackPos = -1) = 0;
	virtual void* get_c_function(int iStackPos = -1) = 0;
	virtual void* get_userdata(int iStackPos = -1) = 0;
	virtual void		push_nil() = 0;
	virtual void		push_string(const char* val, unsigned int iLen = 0) = 0;
	virtual void		push_number(double val) = 0;
	virtual void		push_bool(bool val) = 0;
	virtual void		push_c_function(void* val) = 0;
	virtual void		push_c_closure() = 0;
	virtual void		push_userdata(void*) = 0;
	virtual int			reference_create() = 0;
	virtual void		reference_free(int i) = 0;
	virtual void		reference_push(int i) = 0;
	virtual void		push_special(int iType) = 0;
	virtual bool			is_type(int iStackPos, int iType) = 0;
	virtual int				get_type(int iStackPos) = 0;
	virtual const char* get_type_name(int iType) = 0;
	virtual void			create_meta_table_type(const char* strName, int iType) = 0;
	virtual const char* check_string(int iStackPos = -1) = 0;
	virtual double	check_number(int iStackPos = -1) = 0;
	virtual void obj_len(int) = 0;
	virtual void get_angle(int) = 0;
	virtual void* get_vector(int) = 0;
	virtual void push_angle(void*) = 0;
	virtual void push_vector(void*) = 0;
	virtual void set_state() = 0;
	virtual void create_meta_table(char const*) = 0;
	virtual void push_meta_table(int) = 0;
	virtual void push_user_type(void*, int) = 0;
	virtual void set_user_type(int, void*) = 0;
	virtual void init(void*, bool) = 0;
	virtual void shutdown(void) = 0;
	virtual void cycle(void) = 0;
	virtual void global(void) = 0;
	virtual i_lua_object* get_object(int) = 0;
	virtual void push_lua_object(void*) = 0;
	virtual void push_lua_function() = 0;
	virtual void lua_error(char const*, int) = 0;
	virtual void type_error(char const*, int) = 0;
	virtual void call_internal(int, int) = 0;
	virtual void call_internal_no_returns(int) = 0;
	virtual bool call_internal_get_bool(int) = 0;
	virtual void call_internal_get_string(int) = 0;
	virtual void call_internal_get(int, void*) = 0;
	virtual void new_global_table(char const*) = 0;
	virtual void new_temporary_object(void) = 0;
	virtual void is_user_data(int) = 0;
	virtual void get_meta_table_object(char const*, int) = 0;
	virtual void get_meta_table_object(int) = 0;
	virtual void get_return(int) = 0;
	virtual void is_server(void) = 0;
	virtual void is_client(void) = 0;
	virtual void is_dedicated_server(void) = 0;
	virtual void destroy_object(void*) = 0;
	virtual void create_object(void) = 0;
	virtual void set_member(void*, void*, void*) = 0;
	virtual void get_new_table(void) = 0;
	virtual void set_member(void*, float) = 0;
	virtual void set_member(void*, float, void*) = 0;
	virtual void set_member(void*, char const*) = 0;
	virtual void set_member(void*, char const*, void*) = 0;
	virtual void set_is_server(bool) = 0;
	virtual void push_long(long) = 0;
	virtual void get_flags(int) = 0;
	virtual void find_on_objects_meta_table(int, int) = 0;
	virtual void find_object_on_table(int, int) = 0;
	virtual void set_member_fast(void*, int, int) = 0;
	virtual void run_string(char const* filename, char const* path, char const* stringtoun, bool run = true, bool showerrors = true) = 0;
};


class c_lua_shared
{
public:
    c_lua_interface* get_interface(int state)
    {
        using fn = c_lua_interface * (__thiscall*)(c_lua_shared*, int);
        return (*(fn**)this)[6](this, state);
    }
};



class c_lua_auto_pop
{
	c_lua_interface* i;
	int top;
public:
	c_lua_auto_pop(c_lua_interface* intr);
	~c_lua_auto_pop();
};

inline c_lua_auto_pop::c_lua_auto_pop(c_lua_interface* intr)
{
	top = intr->top();
	i = intr;
}

inline c_lua_auto_pop::~c_lua_auto_pop()
{
	i->pop(i->top() - top);
}
