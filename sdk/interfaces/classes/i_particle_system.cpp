#include "i_particle_system.h"

void i_particle_system::create_effect_index(unsigned int* effect_index, particle_effect * effect_data)
{
    g_sig->get(_("create_effect_index")).as<void(__fastcall*)(i_particle_system*, unsigned int*, particle_effect*)>()(this, effect_index, effect_data);
}

void i_particle_system::create_effect(unsigned int effect_index, int unk, void* clr, int unk2)
{
    g_sig->get(_("create_effect")).as<void(__fastcall*)(i_particle_system*, unsigned int, int, void*, int)>()(this, effect_index, unk, clr, unk2);
}

void i_particle_system::unk_function(int effect_index, unsigned int unk, const c_strong_handle<c_particle_snapshot>* particle_snapshot)
{
    g_sig->get(_("unk_function")).as<bool(__fastcall*)(i_particle_system*, int, unsigned int, const c_strong_handle<c_particle_snapshot>*)>()(this, effect_index, unk, particle_snapshot);
}