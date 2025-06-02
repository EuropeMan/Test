#ifndef VALVE_ENGINE_TRACE
#define VALVE_ENGINE_TRACE

#include "../../../utils/utils.hh"
inline static constexpr std::uint32_t k_main_pen_mask = 0x1C300Bu;

class c_player_pawn;

struct surface_data_t
{
private:
    std::uint8_t padding_0[0x8]{ };
public:
    float m_penetration_modifier{ };
    float m_damage_modifier{ };
private:
    std::uint8_t padding_1[0x4]{ };
public:
    int m_material{ };
};

class trace_hitbox_data_t
{
public:
    const char* m_name; // 0x0
    const char* m_surface_property; // 0x8
    const char* m_bone_name; // 0x10
    vec3_t m_mins; // 0x18
    vec3_t m_maxs; // 0x24
    float m_shape_radius; // 0x30
    uint32_t m_bone_name_hash; // 0x34	
    int32_t m_hitgroup; // 0x38	
    uint8_t m_shape_type; // 0x3c	
    bool m_translation_only; // 0x3d	
private:
    std::uint8_t padding_3[0x2]{ }; // 0x3e
    char pad_0x40[12]{ };
public:
    uint32_t m_crc; // 0x40	
    int32_t m_render_color; // 0x44
    uint16_t m_hitbox_index; // 0x48	
};

struct game_trace_t
{
public:
    void* m_surface{ };
    c_player_pawn* m_hit_entity{ };
    trace_hitbox_data_t* m_hitbox_data{ };
private:
    std::uint8_t padding_0[0x38]{ };
public:
    std::uint32_t m_contents{ };
private:
    std::uint8_t padding_1[0x24]{ };
public:
    vec3_t m_start_pos{ };
    vec3_t m_end_pos{ };
    vec3_t m_normal{ };
    vec3_t m_pos{ };
private:
    std::uint8_t padding_2[0x4]{ };
public:
    float m_fraction{ };
private:
    std::uint8_t padding_3[0x6]{ };
public:
    bool m_all_solid{ };
private:
    std::uint8_t padding_4[0x4D]{ };
};

class trace_filter_t
{
public:
    std::uintptr_t* ptr; // 0x0
    std::uint64_t trace_mask; // 0x8
    std::uint64_t unk1;
    std::uint64_t unk2;
    int skip_handle1; // 0x20
    int skip_handle2; // 0x24
    int skip_owner_handle1;
    int skip_owner_handle2;
    std::uint16_t collision1; // 0x30
    std::uint16_t collision2; // 0x32
    std::uint16_t N0000011C; // 0x34
    std::uint8_t layer; // 0x36
    std::uint8_t flags; // 0x37
    std::uint8_t null_it3; // 0x38
    trace_filter_t(std::uint64_t trace_mask, c_player_pawn* local_player, c_player_pawn* entity, std::uint8_t layer);
};

struct update_value_t
{
public:
    float m_previous_length_mod{ };
    float m_current_length_mod{ };
private:
    std::uint8_t padding_0[0x8]{ };
public:
    std::int16_t m_handle_idx{ };
private:
    std::uint8_t padding_1[0x6]{ };
};

struct trace_arr_element_t
{
public:
    std::uint8_t padding_0[0x30]{ };
};

class c_trace_info
{
public:
    float unk;
    float distance;
    float damage;
    std::uint32_t penetration_count;
    std::uint32_t handle;
    std::uint32_t handle2;
};

class c_trace_data
{
public:
    char pad[0x8];
    void* potroshitel;
    char pad5[0x1810];
    int size;
    c_trace_info* trace_info;
    char pad2[0x8];
    void* unk;
    char pad4[184];
    vec3_t start;
    vec3_t end;
    char pad3[0x50];
};

struct ray_t
{
    vec3_t m_start{ };
    vec3_t m_end{ };
    vec3_t m_mins{ };
    vec3_t m_maxs{ };
private:
    std::uint8_t padding_0[0x4]{ };
public:
    std::uint8_t m_unk_type{ };
};

class i_engine_trace
{
public:
    void init_trace_info(game_trace_t* const hit)
    {
        static const auto sig = g_sig->get(_("m_init_trace_info")).as<void(__fastcall*)(game_trace_t*) >();
        sig(hit);
    }

    void init_trace_filter(trace_filter_t& filter, c_player_pawn* skip, uint64_t mask, uint8_t layer, uint16_t idk)
    {
        static const auto sig = g_sig->get(_("m_init_trace_filter")).as<trace_filter_t * (__fastcall*)(trace_filter_t&, void*, uint64_t, uint8_t, uint16_t) >();
        sig(filter, skip, mask, layer, idk);
    }

    void clip_trace_to_players(vec3_t& start, vec3_t& end, trace_filter_t* filter, game_trace_t* trace, float min, int length, float max)
    {
        static const auto sig = g_sig->get(_("m_clip_trace_to_players")).as<void(__fastcall*)(vec3_t&, vec3_t&, trace_filter_t*, game_trace_t*, float, int, float)>();
        sig(start, end, filter, trace, min, max, length);
    }

    static void get_trace_info(c_trace_data* trace, game_trace_t* hit, const float unknown_float, void* unknown)
    {
        static const auto sig = g_sig->get(_("m_get_trace_info")).as<void(__fastcall*)(c_trace_data*, game_trace_t*, float, void*) >();
        return sig(trace, hit, unknown_float, unknown);
    }

    static bool handle_bullet_penetration(c_trace_data* const trace, void* stats, update_value_t* const mod_value, const bool draw_showimpacts = false)
    {
        static const auto sig = g_sig->get(_("m_handle_bullet_pen")).as<bool(__fastcall*)(c_trace_data*, void*, update_value_t*, void*, void*, void*, void*, void*, bool) >();
        return sig(trace, stats, mod_value, nullptr, nullptr, nullptr, nullptr, nullptr, draw_showimpacts);
    }

    static void create_trace(c_trace_data* const trace, const vec3_t start, const vec3_t end, const trace_filter_t& filler, const int penetration_count)
    {
        static const auto sig = g_sig->get(_("m_create_trace")).as<void(__fastcall*)(c_trace_data*, vec3_t, vec3_t, trace_filter_t, void*, void*, void*, void*, int) >();
        return sig(trace, start, end, filler, nullptr, nullptr, nullptr, nullptr, penetration_count);
    }

    void trace_shape(ray_t& ray, vec3_t* start, vec3_t* end, trace_filter_t filter, game_trace_t& trace)
    {
        static const auto sig = g_sig->get(_("m_trace_shape")).as<bool(__fastcall*)(void*, ray_t&, vec3_t*, vec3_t*, trace_filter_t, game_trace_t&)>();
        sig(this, ray, start, end, filter, trace);
    }
};

#endif