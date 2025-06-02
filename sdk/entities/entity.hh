#ifndef VALVE_BASE_ENTITY
#define VALVE_BASE_ENTITY

#include "../misc/base_handle.hh"
#include "../schema/schema_system.hh"
#include "../misc/bits.hh"
#include "../../utils/math/strong_handle.hh"

struct name_vfx
{
    const char* name1;
    const char* name2;
};

class c_hitbox
{
public:
    const char* m_name; // 0x0	
    const char* m_sSurfaceProperty; // 0x8	
    const char* m_sBoneName; // 0x10	
    vec3_t m_vMinBounds; // 0x18	
    vec3_t m_vMaxBounds; // 0x24	
    float m_flShapeRadius; // 0x30	
    uint32_t m_nBoneNameHash; // 0x34	
    int32_t m_nGroupId; // 0x38	
    uint8_t m_nShapeType; // 0x3c	
    bool m_bTranslationOnly; // 0x3d	
private:
    uint8_t __pad003e[0x2]; // 0x3e
public:
    uint32_t m_CRC; // 0x40	
    c_color m_cRenderColor; // 0x44	
    uint16_t m_nHitBoxIndex; // 0x48
    char pad2[0x26];
};

struct scene_hitbox
{
    c_hitbox* hitbox;
    char pad[0x8];
};

struct scene_hitbox_data
{
    std::uint64_t hitbox_count{};
    void* hitbox_data{};
    char pad[0x8];
    scene_hitbox hitbox_array[19]{};
};

struct hitbox_position
{
    vec3_t position{};
    float scale{};
    vec4_t rotate{};
};

struct scene_hitbox_position
{
    std::uint64_t hitbox_count{};
    char pad[0x8]{};
    void* hitbox_data{};
    char pad2[0x8];
    hitbox_position hitbox_array[19]{};
};

class c_entity_identity
{
public:
    SCHEMA(m_name_stringable_index, int, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_nameStringableIndex")));
    SCHEMA(m_name, const char*, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_name")));
    SCHEMA(m_designer_name, const char*, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_designerName")));
    SCHEMA(m_flags, std::uint32_t, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_flags")));
    SCHEMA(m_data_object_types, std::uint32_t, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_fDataObjectTypes")));
    SCHEMA(m_prev, c_entity_identity*, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_pPrev")));
    SCHEMA(m_next, c_entity_identity*, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_pNext")));
    SCHEMA(m_prev_by_class, c_entity_identity*, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_pPrevByClass")));
    SCHEMA(m_next_by_class, c_entity_identity*, fnv1a::hash(_("CEntityIdentity")), fnv1a::hash(_("m_pNextByClass")));

    OFFSET(int, m_get_index, 0x10);
};

class c_script_component
{
public:
    SCHEMA(m_script_class_name, const char*, fnv1a::hash(_("CScriptComponent")), fnv1a::hash(_("m_scriptClassName")));
};

class c_entity_instance
{
public:
    SCHEMA(m_private_vscripts, const char*, fnv1a::hash(_("CEntityInstance")), fnv1a::hash(_("m_iszPrivateVScripts")));
    SCHEMA(m_entity, c_entity_identity*, fnv1a::hash(_("CEntityInstance")), fnv1a::hash(_("m_pEntity")));
    SCHEMA(m_script_component, c_script_component*, fnv1a::hash(_("CEntityInstance")), fnv1a::hash(_("m_CScriptComponent")));
    SCHEMA(m_visible_in_pvs, c_entity_identity*, fnv1a::hash(_("CEntityInstance")), fnv1a::hash(_("m_bVisibleinPVS")));

    VFUNC(void(__thiscall*)(void*, schema_class_info_data_t**), get_schema_class_info(schema_class_info_data_t** name), 38, name);

    c_base_handle get_ref_ehandle()
    {
        c_base_handle handle{};
        c_entity_identity* identity = this->m_entity();
        if (!identity)
            return handle;

        c_base_handle m_unk_handle = c_base_handle(identity->m_get_index());
        handle.init(m_unk_handle.get_entry_index(), m_unk_handle.get_serial_number() - (identity->m_flags() & 1));

        return handle;
    }

    schema_class_info_data_t* get_schema_class()
    {
        schema_class_info_data_t* class_info = nullptr;
        this->get_schema_class_info(&class_info);
        if (!class_info)
            return nullptr;

        return class_info;
    }
};

class c_collision_property
{
public:
    SCHEMA(m_vec_mins, vec3_t, fnv1a::hash(_("CCollisionProperty")), fnv1a::hash(_("m_vecMins")));
    SCHEMA(m_vec_maxs, vec3_t, fnv1a::hash(_("CCollisionProperty")), fnv1a::hash(_("m_vecMaxs")));
};

struct alignas(16) c_bone_data
{
    vec3_t m_pos;
    float m_scale;
    vec4_t m_rot;
};

class c_model
{
public:
    bits_t get_bone_flags(std::uint32_t index)
    {
        static const auto sig = g_sig->get(_("get_bone_flags")).as< std::uint32_t(__fastcall*)(void*, std::uint32_t) >();
        return sig(this, index);
    }

    const char* get_bone_name(std::uint32_t index)
    {
        static const auto sig = g_sig->get(_("get_bone_name")).as< const char*(__fastcall*)(void*, std::uint32_t) >();
        return sig(this, index);
    }

    std::int32_t get_bone_parent(std::uint32_t index)
    {
        static const auto sig = g_sig->get(_("get_bone_parent")).as< std::int32_t(__fastcall*)(void*, std::uint32_t) >();
        return sig(this, index);
    }
};

class c_model_state
{
public:
    SCHEMA(m_model, c_strong_handle<c_model>, fnv1a::hash(_("CModelState")), fnv1a::hash(_("m_hModel")));

    c_bone_data* get_bone_data() noexcept
    {
        return *reinterpret_cast<c_bone_data**>(reinterpret_cast<std::uintptr_t>(this) + 0x80);
    }
};

class c_skeleton_instance
{
public:
    char pad_0000[0x1BC]; //0x0000
    int m_bones; //0x01BC 
    char pad_01C0[0x18]; //0x01C0
    int m_mask; //0x01D8 
    char pad_01DC[0x4]; //0x01DC
    matrix4x2_t* m_bone_cache; //0x01E0

    SCHEMA(m_model_state, c_model_state, fnv1a::hash(_("CSkeletonInstance")), fnv1a::hash(_("m_modelState")));
    SCHEMA(m_hitbox_set, std::uint8_t, fnv1a::hash(_("CSkeletonInstance")), fnv1a::hash(_("m_nHitboxSet")));

    void get_bone(c_bone_data& data, int index)
    {
        static const auto sig = g_sig->get(_("get_bone")).as< void(__fastcall*)(void*, c_bone_data&, int) >();
        return sig(this, data, index);
    }

    void calc_world_space_bones(std::uint32_t bone_mask)
    {
        static const auto sig = g_sig->get(_("calc_world_space_bones")).as< void(__fastcall*)(void*, unsigned int) >();
        return sig(this, bone_mask);
    }
};

class c_game_scene_node
{
public:
    SCHEMA(m_vec_abs_origin, vec3_t, fnv1a::hash(_("CGameSceneNode")), fnv1a::hash(_("m_vecAbsOrigin")));
    SCHEMA(m_vec_origin, vec3_t, fnv1a::hash(_("CGameSceneNode")), fnv1a::hash(_("m_vecOrigin")));
    SCHEMA(m_render_origin, vec3_t, fnv1a::hash(_("CGameSceneNode")), fnv1a::hash(_("m_vRenderOrigin")));
    SCHEMA(m_ang_rotation, vec3_t, fnv1a::hash(_("CGameSceneNode")), fnv1a::hash(_("m_angRotation")));
    SCHEMA(m_ang_abs_rotation, vec3_t, fnv1a::hash(_("CGameSceneNode")), fnv1a::hash(_("m_angAbsRotation")));
    SCHEMA(m_dormant, bool, fnv1a::hash(_("CGameSceneNode")), fnv1a::hash(_("m_bDormant")));

    VFUNC(c_skeleton_instance* (__thiscall*)(void*), get_skeleton(), 8);
};

class c_base_entity : public c_entity_instance
{
public:
    SCHEMA(m_team_num, int, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_iTeamNum")));
    SCHEMA(m_fflags, bits_t, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_fFlags")));
    SCHEMA(m_health, int, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_iHealth")));
    SCHEMA(m_life_state, int, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_lifeState")));
    SCHEMA(m_vec_velocity, vec3_t, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_vecVelocity")));
    SCHEMA(m_vec_abs_velocity, vec3_t, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_vecAbsVelocity")));
    SCHEMA(m_game_scene_node, c_game_scene_node*, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_pGameSceneNode")));
    SCHEMA(m_p_collision, c_collision_property*, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_pCollision")));
    SCHEMA(m_move_type, int, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_MoveType")));
    SCHEMA(m_actual_move_type, int, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_nActualMoveType")));
    SCHEMA(m_simulation_time, float, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_flSimulationTime")));

    SCHEMA(m_owner_entity, c_base_handle, fnv1a::hash(_("C_BaseEntity")), fnv1a::hash(_("m_hOwnerEntity")));

    VFUNC(bool(__thiscall*)(void*), m_is_player(), 143);
    VFUNC(bool(__thiscall*)(void*), m_is_weapon(), 150);

    void set_abs_origin(vec3_t abs_origin)
    {
        static auto sig = g_sig->get(_("set_abs_origin")).as< void(__fastcall*)(void*, const vec3_t&) >();
        return sig(this, abs_origin);
    }
};

class c_base_model_entity : public c_base_entity
{
public:

};

#endif