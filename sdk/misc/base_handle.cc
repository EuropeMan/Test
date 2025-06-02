#include "base_handle.hh"
#include "../interfaces/interfaces.hh"
#include "../entities/entity.hh"

c_base_entity* c_base_handle::get() const
{
    if (!is_valid())
        return nullptr;

    c_base_entity* ent = static_cast<c_base_entity*>(g_interfaces->m_game_resource_service->game_entity_system->get_entity_by_index(get_entry_index()));
    if (!ent || ent->get_ref_ehandle() != *this)
        return nullptr;

    return ent;
}