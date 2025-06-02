#ifndef CSGO_ENGINE_CVAR_SYS
#define CSGO_ENGINE_CVAR_SYS

#define fcvar_none                0 
#define fcvar_unregistered              (1<<0)  // If this is Set, don't add to linked list, etc.
#define fcvar_development_only          (1<<1)  // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define fcvar_game_dll                  (1<<2)  // defined by the game DLL
#define fcvar_client_dll                (1<<3)  // defined by the client DLL
#define fcvar_hidden                    (1<<4)  // Hidden. Doesn't appear in GetOffset or auto complete. Like development_only, but can't be compiled out.                              
#define fcvar_protected                 (1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define fcvar_sponly                    (1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define fcvar_archive                   (1<<7)  // Set to cause it to be saved to vars.rc
#define fcvar_notify                    (1<<8)  // notifies players when changed
#define fcvar_user_info                 (1<<9)  // changes the client's info string
#define fcvar_printable_only            (1<<10) // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define fcvar_unlogged                  (1<<11) // If this is a fcvar_server, don't log changes to the log file / console if we are creating a log
#define fcvar_never_as_string           (1<<12) // never try to print that cvar
#define fcvar_replicated                (1<<13) // server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define fcvar_demo                      (1<<16) // record this cvar when starting a demo file
#define fcvar_dont_record               (1<<17) // don't record these command in demofiles
#define fcvar_ss_added                  (1<<18) // This is one of the "added" fcvar_ss variables for the splitscreen players
#define fcvar_release                   (1<<19) // Cvars tagged with this are the only cvars avaliable to customers
#define fcvar_reload_materials          (1<<20) // If this cvar changes, it forces a material reload
#define fcvar_reload_textures           (1<<21) // If this cvar changes, if forces a texture reload
#define fcvar_not_connected             (1<<22) // cvar cannot be changed by a client that is connected to a server
#define fcvar_material_system_thread    (1<<23) // Indicates this cvar is read from the material system thread
#define fcvar_archive_xbox              (1<<24) // cvar written to config.cfg on the Xbox
#define fcvar_accessible_from_threads   (1<<25) // used as a debugging tool necessary to check material system thread convars
#define fcvar_server_can_execute        (1<<28) // the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define fcvar_server_cannot_query       (1<<29) // If this is Set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define fcvar_client_cmd_can_execute    (1<<30) // IVEngineClient::ClientCmd is allowed to execute this command. 

#include <cstdint>
#include "../../../../deps/fnv1a/fnv1a.h"
#include "../../../utils/color/color.hh"
#include "../../../utils/math/vector.hh"
#include "../../../utils/memory/memory.hh"

union c_convar_value
{
	bool i1;
	short i16;
	std::uint16_t u16;
	int i32;
	std::uint32_t u32;
	std::int64_t i64;
	std::uint64_t u64;
	float fl;
	double db;
	const char* sz;
	c_color clr;
	vec2_t vec2;
	vec3_t vec3;
	vec4_t vec4;
	vec3_t ang;
};

class i_convar
{
public:
	const char* m_name; // 0x0000
	i_convar* m_next; // 0x0008
	std::uint8_t padding_1[0x10]{ }; // 0x0010
	const char* m_description; // 0x0020
	std::uint32_t m_type; // 0x28
	std::uint32_t m_registered; // 0x2C
	std::uint32_t m_flags; // 0x30
	std::uint8_t padding_2[0x8]{ }; // 0x34
	c_convar_value m_value; // 0x40

	bool get_bool() { return m_value.i1; }
	int get_int() { return m_value.i32; }
	float get_float() { return m_value.fl; }

	bool set_bool(bool value) { return m_value.i1 = value; }
	int set_int(int value) { return m_value.i32 = value; }
	float set_float(float value) { return m_value.fl = value; }
};

class i_cvar
{
public:
	VFUNC(void* (__thiscall*)(void*, std::uint64_t&), first_cvar_iterator(std::uint64_t& idx), 12, idx);
	VFUNC(void* (__thiscall*)(void*, std::uint64_t&, std::uint64_t), next_cvar_iterator(std::uint64_t& idx1, std::uint64_t idx2), 13, idx1, idx2);
	VFUNC(i_convar* (__thiscall*)(void*, std::uint64_t), find_var_by_index(std::uint64_t idx), 37, idx);

	i_convar* find_var(std::uint32_t var_hash)
	{
		std::uint64_t i = 0;
		first_cvar_iterator(i);
		while (i != 0xFFFFFFFF)
		{
			i_convar* convar = find_var_by_index(i);
			if (convar && fnv1a::hash(convar->m_name) == var_hash)
				return convar;
			next_cvar_iterator(i, i);
		}
		return nullptr;
	}

	void unlock_hidden_cvars()
	{
		std::uint64_t i = 0;
		first_cvar_iterator(i);
		while (i != 0xFFFFFFFF)
		{
			i_convar* convar = find_var_by_index(i);
			if (convar)
			{
				if (convar->m_flags & fcvar_hidden)
					convar->m_flags &= ~fcvar_hidden;

				if (convar->m_flags & fcvar_development_only)
					convar->m_flags &= ~fcvar_development_only;
			}
			next_cvar_iterator(i, i);
		}
	}
};

#endif