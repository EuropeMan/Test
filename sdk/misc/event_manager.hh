#ifndef VALVE_EVENT_MANAGER
#define VALVE_EVENT_MANAGER

#include "../../utils/memory/memory.hh"
#include "../../../signatures.hh"
#include "../entities/player.hh"

struct key_string_t
{
    std::uint64_t m_key;
    const char* m_name;

    key_string_t(std::uint64_t key, const char* name) : m_key(key), m_name(name) { }
    key_string_t(const char* name, bool should_find_key = false) : m_name(name) { m_key = should_find_key ? find_key(name) : 0x0; }

    std::uint64_t find_key(const char* name)
    {
        static const auto sig = g_sig->get(_("m_find_key")).as< std::uint64_t(__fastcall*)(const char*, unsigned int, int) >();
        return sig(name, 0x12, 0x31415926);
    }
};

struct events_t
{
    VFUNC(const char* (__thiscall*)(void*), get_name(), 1);
    VFUNC(bool(__thiscall*)(void*, key_string_t*, bool), handle_bool(key_string_t* token_name), 6, token_name, false);
    VFUNC(int(__thiscall*)(void*, key_string_t*, int), handle_int_from_hash(key_string_t* token_name), 7, token_name, 0);
    VFUNC(float(__thiscall*)(void*, key_string_t*, float), handle_float(key_string_t* token_name), 9, token_name, 0.0f);
    VFUNC(const char* (__thiscall*)(void*, key_string_t*, const char*), handle_string(key_string_t* token_name), 10, token_name, nullptr);
    VFUNC(c_player_controller* (__thiscall*)(void*, key_string_t*), handle_player_controller(key_string_t* token_name), 16, token_name);


    bool get_bool(const std::string_view token_name) noexcept
    {
        key_string_t token(-1, token_name.data());
        return handle_bool(&token);
    }

    int get_int_from_hash(const std::string_view token_name) noexcept
    {
        key_string_t token(-1, token_name.data());
        return handle_int_from_hash(&token);
    }

    float get_float(const std::string_view token_name) noexcept
    {
        key_string_t token(-1, token_name.data());
        return handle_float(&token);
    }

    const char* get_string(const std::string_view token_name) noexcept
    {
        key_string_t token(-1, token_name.data());
        return handle_string(&token);
    }

    c_player_controller* get_player_controller(const std::string_view token_name) noexcept
    {
        key_string_t token(-1, token_name.data());
        return handle_player_controller(&token);
    }

    void* get_player_pawn_from_id(const std::string_view event_name) noexcept
    {
        static const auto sig = g_sig->get(_("get_player_pawn_from_id")).as< void* (__fastcall*)(void*, const char*, std::int64_t) >();
        return sig(this, event_name.data(), 0);
    }

    std::int64_t get_int(const std::string_view event_name) noexcept
    {
        static const auto sig = g_sig->get(_("get_int_for_event")).as< std::int64_t(__fastcall*)(void*, const char*) >();
        return sig(this, event_name.data());
    }
};

#endif