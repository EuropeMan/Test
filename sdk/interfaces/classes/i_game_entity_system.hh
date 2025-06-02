#ifndef CSGO_GAME_ENTITY_SYS
#define CSGO_GAME_ENTITY_SYS

#include "../../misc/base_handle.hh"
#include "../../../../deps/xor/xor.h"
#include "../../../../signatures.hh"

class c_player_pawn;

class c_game_entity_system
{
public:
	unsigned int get_handle_entity(c_player_pawn* entity)
	{
		if (!entity)
			return 0xFFFFFFFF;

		static const auto sig = g_sig->get(_("m_get_handle_entity")).as< unsigned int(__fastcall*)(c_player_pawn*) >();
		return sig(entity);
	}

	int get_colision_entity(c_player_pawn* entity)
	{
		if (!entity)
			return -1;

		static const auto sig = g_sig->get(_("m_get_collision_entity")).as< int(__fastcall*)(c_player_pawn*) >();
		return sig(entity);
	}

	int get_highest_entity_index()
	{
		static const auto sig = g_sig->get(_("get_highest_entity_index")).abs().as< int(__thiscall*) (void*, int*) >();

		int idx = -1;
		if (!sig)
			return idx;

		sig(this, &idx);
		return idx;
	}

	void* get_entity_by_index(int entity_index)
	{
		static const auto sig = g_sig->get(_("get_entity_by_index")).as< void* (__thiscall*) (void*, int) >();
		return sig(this, entity_index);
	}

	template <typename T = c_base_entity>
	T* get(const c_base_handle handle)
	{
		if (!handle.is_valid())
			return nullptr;

		return reinterpret_cast<T*>(this->get_entity_by_index(handle.get_entry_index()));
	}

	template <typename T = c_base_entity>
	T* get(int index)
	{
		return reinterpret_cast<T*>(this->get_entity_by_index(index));
	}

	c_player_pawn* get_entity(int index)
	{
		__int64 v2; // rcx
		__int64 v3; // r8
		__int64 result{}; // rax

		if ((unsigned int)index <= 0x7FFE
			&& (unsigned int)(index >> 9) <= 0x3F
			&& (v2 = *(std::uintptr_t*)(std::uintptr_t(this) + 8 * (index >> 9) + 16)) != 0
			&& (v3 = 120 * (index & 0x1FF), v3 + v2)
			&& (*(std::uintptr_t*)(v3 + v2 + 16) & 0x7FFF) == index)
		{
			result = *(std::uintptr_t*)(v3 + v2);
		}
		return reinterpret_cast<c_player_pawn*>(result);
	}
};

class i_game_resource_service
{
public:
	std::uint8_t padding_1[0x58]{ };
	c_game_entity_system* game_entity_system;
};

#endif