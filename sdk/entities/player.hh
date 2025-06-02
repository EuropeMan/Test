#ifndef VALVE_BASE_PLAYER
#define VALVE_BASE_PLAYER

#include "entity.hh"
#include "weapon.hh"
#include "../interfaces/interfaces.hh"

#define get_hash(str) fnv1a::hash(str)

class c_player_pawn;

using c_firing_float_t = std::float_t[2];
using c_skill_float_t = std::float_t[4];

class post_processing_settings
{
public:
	char pad[0xBD];
	bool shadow;
};

class unknown
{
public:
	std::uintptr_t* get_penis()
	{
		return (std::uintptr_t*)(this + 0x310);
	}
};

class scene_view
{
public:
	char pad[0x38];
	unknown* unk_;
};

class c_cs_weapon_base_vdata
{
public:
	SCHEMA(m_imaxclip1, int32_t, get_hash("CBasePlayerWeaponVData"), get_hash("m_iMaxClip1"));
	SCHEMA(m_imaxclip2, int32_t, get_hash("CBasePlayerWeaponVData"), get_hash("m_iMaxClip2"));
	SCHEMA(m_idefaultclip1, int32_t, get_hash("CBasePlayerWeaponVData"), get_hash("m_iDefaultClip1"));
	SCHEMA(m_idefaultclip2, int32_t, get_hash("CBasePlayerWeaponVData"), get_hash("m_iDefaultClip2"));
	SCHEMA(m_iweight, int32_t, get_hash("CBasePlayerWeaponVData"), get_hash("m_iWeight"));
	SCHEMA(m_weapon_name, const char*, get_hash("CCSWeaponBaseVData"), get_hash("m_szName"));
	SCHEMA(m_fl_cycle_time, c_firing_float_t, get_hash("CCSWeaponBaseVData"), get_hash("m_flCycleTime"));
	SCHEMA(m_damage, int, get_hash("CCSWeaponBaseVData"), get_hash("m_nDamage"));
	SCHEMA(m_armor_ratio, float, get_hash("CCSWeaponBaseVData"), get_hash("m_flArmorRatio"));
	SCHEMA(m_range, float, get_hash("CCSWeaponBaseVData"), get_hash("m_flRange"));
	SCHEMA(m_range_modifier, float, get_hash("CCSWeaponBaseVData"), get_hash("m_flRangeModifier"));
	SCHEMA(m_penetration, float, get_hash("CCSWeaponBaseVData"), get_hash("m_flPenetration"));
	SCHEMA(m_headshot_multiplier, float, get_hash("CCSWeaponBaseVData"), get_hash("m_flHeadshotMultiplier"));
	SCHEMA(m_spread, float, get_hash("CCSWeaponBaseVData"), get_hash("m_flSpread"));
	SCHEMA(m_weapon_bullets, float, get_hash("CCSWeaponBaseVData"), get_hash("m_nNumBullets"));
	SCHEMA(m_weapon_type, int, get_hash("CCSWeaponBaseVData"), get_hash("m_WeaponType"));
};

class c_econ_item_view
{
public:
	SCHEMA(m_item_definition_index, uint16_t, get_hash("C_EconItemView"), get_hash("m_iItemDefinitionIndex"));
	SCHEMA(m_item_id, uint64_t, get_hash("C_EconItemView"), get_hash("m_iItemID"));
	SCHEMA(m_item_id_low, uint32_t, get_hash("C_EconItemView"), get_hash("m_iItemIDLow"));
	SCHEMA(m_item_id_high, uint32_t, get_hash("C_EconItemView"), get_hash("m_iItemIDHigh"));
	SCHEMA(m_account_id, uint32_t, get_hash("C_EconItemView"), get_hash("m_iAccountID"));
	SCHEMA(m_initialized, bool, get_hash("C_EconItemView"), get_hash("m_bInitialized"));
	SCHEMA(m_custom_name, const char*, get_hash("C_EconItemView"), get_hash("m_szCustomName"));
	SCHEMA(m_custom_name_override, const char*, get_hash("C_EconItemView"), get_hash("m_szCustomNameOverride"));
};

class c_attribute_container
{
public:
	SCHEMA(m_item, c_econ_item_view, get_hash("C_AttributeContainer"), get_hash("m_Item"));
};

class c_econ_entity : public c_base_model_entity
{
public:
	SCHEMA(m_attribute_manager, c_attribute_container, get_hash("C_EconEntity"), get_hash("m_AttributeManager"));
	SCHEMA(m_fallback_paint_kit, int, get_hash("C_EconEntity"), get_hash("m_nFallbackPaintKit"));
	SCHEMA(m_fallback_seed, int, get_hash("C_EconEntity"), get_hash("m_nFallbackSeed"));
	SCHEMA(m_fallback_wear, float, get_hash("C_EconEntity"), get_hash("m_flFallbackWear"));
	SCHEMA(m_fallback_stat_trak, int, get_hash("C_EconEntity"), get_hash("m_nFallbackStatTrak"));
	SCHEMA(m_need_to_re_apply_gloves, bool, get_hash("C_EconEntity"), get_hash("m_bNeedToReApplyGloves"));
	SCHEMA(m_econ_gloves, c_econ_item_view, get_hash("C_EconEntity"), get_hash("m_EconGloves"));
};

class c_base_player_weapon : public c_econ_entity
{
public:
	SCHEMA(m_next_primary_attack_tick, int32_t, get_hash("C_BasePlayerWeapon"), get_hash("m_nNextPrimaryAttackTick"));
	SCHEMA(m_next_primary_attack_tick_ratio, float, get_hash("C_BasePlayerWeapon"), get_hash("m_flNextPrimaryAttackTickRatio"));
	SCHEMA(m_next_secondary_attack_tick, int32_t, get_hash("C_BasePlayerWeapon"), get_hash("m_nNextSecondaryAttackTick"));
	SCHEMA(m_next_secondary_attack_tick_ratio, float, get_hash("C_BasePlayerWeapon"), get_hash("m_flNextSecondaryAttackTickRatio"));
	SCHEMA(m_iclip1, int32_t, get_hash("C_BasePlayerWeapon"), get_hash("m_iClip1"));
	SCHEMA(m_iclip2, int32_t, get_hash("C_BasePlayerWeapon"), get_hash("m_iClip2"));
	SCHEMA(m_recoil_index, float, get_hash("C_CSWeaponBase"), get_hash("m_flRecoilIndex"));

	__forceinline float get_spread() {
		using original_fn = float(__fastcall*)(void*);
		return (*(original_fn**)this)[351](this);
	}

	__forceinline float get_inaccuracy() {
		using original_fn = float(__fastcall*)(void*);
		return (*(original_fn**)this)[397](this);

	}

	c_cs_weapon_base_vdata* m_weapon_data()
	{
		return *reinterpret_cast<c_cs_weapon_base_vdata**>((uint64_t)this + 0x368);
	}
};

class c_cs_player_item_services
{
public:
	SCHEMA(m_has_defuser, bool, get_hash("CCSPlayer_ItemServices"), get_hash("m_bHasDefuser"));
	SCHEMA(m_has_helmet, bool, get_hash("CCSPlayer_ItemServices"), get_hash("m_bHasHelmet"));
	SCHEMA(m_has_heavy_armor, bool, get_hash("CCSPlayer_ItemServices"), get_hash("m_bHasHeavyArmor"));
};

class c_cs_player_weapon_services
{
public:
	SCHEMA(m_next_attack, float, get_hash("CCSPlayer_WeaponServices"), get_hash("m_flNextAttack"));
	SCHEMA(m_looking_at_weapon, bool, get_hash("CCSPlayer_WeaponServices"), get_hash("m_bIsLookingAtWeapon"));
	SCHEMA(m_holding_look_at_weapon, bool, get_hash("CCSPlayer_WeaponServices"), get_hash("m_bIsHoldingLookAtWeapon"));
};

class c_player_weapon_services : public c_cs_player_weapon_services
{
public:
	SCHEMA(m_active_weapon, c_handle<c_base_player_weapon>, get_hash("CPlayer_WeaponServices"), get_hash("m_hActiveWeapon"));
	SCHEMA(m_last_weapon, c_handle<c_base_player_weapon>, get_hash("CPlayer_WeaponServices"), get_hash("m_hLastWeapon"));
	SCHEMA(m_my_weapons, c_utl_vector<c_handle<c_base_player_weapon>>, get_hash("CPlayer_WeaponServices"), get_hash("m_hMyWeapons"));
};

class c_base_player_controller : public c_base_entity
{
public:
	SCHEMA(m_pawn, c_handle<c_player_pawn>, fnv1a::hash(_("CBasePlayerController")), fnv1a::hash(_("m_hPawn")));
	SCHEMA(m_is_local_player_controller, bool, fnv1a::hash(_("CBasePlayerController")), fnv1a::hash(_("m_bIsLocalPlayerController")));
};

class c_player_controller : public c_base_player_controller
{
public:
    SCHEMA(m_ping, int, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_iPing")));
    SCHEMA(m_tick_base, uint32_t, fnv1a::hash(_("CBasePlayerController")), fnv1a::hash(_("m_nTickBase")));
    SCHEMA(m_pawn_health, int, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_iPawnHealth")));
    SCHEMA(m_pawn_armor, int, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_iPawnArmor")));
    SCHEMA(m_pawn_has_defuser, bool, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_bPawnHasDefuser")));
    SCHEMA(m_pawn_has_helmet, bool, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_bPawnHasHelmet")));
    SCHEMA(m_pawn_is_alive, bool, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_bPawnIsAlive")));
    SCHEMA(m_sanitized_player_name, const char*, fnv1a::hash(_("CCSPlayerController")), fnv1a::hash(_("m_sSanitizedPlayerName")));
};

class c_camera_services
{
public:
	SCHEMA(m_post_processing_volumes, c_utl_vector<c_base_handle>, fnv1a::hash(_("CPlayer_CameraServices")), fnv1a::hash(_("m_PostProcessingVolumes")));
};

class c_base_player_pawn : public c_base_model_entity
{
public:
	SCHEMA(m_weapon_services, c_player_weapon_services*, get_hash("C_BasePlayerPawn"), get_hash("m_pWeaponServices"));
	SCHEMA(m_item_services, c_cs_player_item_services*, fnv1a::hash(_("C_BasePlayerPawn")), fnv1a::hash(_("m_pItemServices")));

	SCHEMA(m_camera_services, c_camera_services*, fnv1a::hash(_("C_BasePlayerPawn")), fnv1a::hash(_("m_pCameraServices")));
};

class c_player_pawn_base : public c_base_player_pawn
{
public:
    c_base_player_weapon* get_active_weapon()
    {
        if (m_weapon_services() && m_weapon_services()->m_active_weapon().is_valid())
        {
            auto index = m_weapon_services()->m_active_weapon().get_entry_index() & 0x7fff;
            auto wpn = g_interfaces->m_game_resource_service->game_entity_system->get<c_base_player_weapon>(index);
            if (wpn && wpn->m_is_weapon())
            {
                return wpn;
            }
        }

        return nullptr;
    }
};

class c_player_pawn : public c_player_pawn_base
{
public:
	SCHEMA(m_aim_punch_cache, c_utl_vector<vec3_t>, get_hash("C_CSPlayerPawn"), get_hash("m_aimPunchCache"))
	SCHEMA(m_armor_value, int, fnv1a::hash(_("C_CSPlayerPawn")), fnv1a::hash(_("m_ArmorValue")));
	SCHEMA(m_wait_for_no_attack, bool, fnv1a::hash(_("C_CSPlayerPawn")), fnv1a::hash(_("m_bWaitForNoAttack")));

	// hahahahahaahah @miracle
	inline bool is_player_pawn() {
		auto class_info = this->get_schema_class();
		if (class_info == nullptr)
			return false;

		if (fnv1a::hash(class_info->m_name) != fnv1a::hash(_("C_CSPlayerPawn")))
			return false;

		return true;
	}

    bool m_is_friend();

    __forceinline vec3_t get_eye_angle() {
        vec3_t eye_position;

        CALL_VIRTUAL(std::uint64_t, 160, this, &eye_position);

        return eye_position;
    }
};

#endif