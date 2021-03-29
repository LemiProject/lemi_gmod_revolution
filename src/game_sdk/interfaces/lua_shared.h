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
	virtual void* get_object(int) = 0;
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