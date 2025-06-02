#include "../../../../globals.hh"

trace_filter_t::trace_filter_t(std::uint64_t trace_mask, c_player_pawn* local_player, c_player_pawn* entity, std::uint8_t layer) {
    static auto pointer = g_sig->get(_("m_trace_filter_vtable"));

    this->ptr = pointer.as<std::uintptr_t*>();
    this->trace_mask = trace_mask;

    if (local_player)
    {
        this->skip_handle1 = g_interfaces->m_game_resource_service->game_entity_system->get_handle_entity(local_player);
        this->skip_owner_handle1 = g_interfaces->m_game_resource_service->game_entity_system->get_handle_entity(g_interfaces->m_game_resource_service->game_entity_system->get_entity(local_player->m_owner_entity().get_entry_index()));
        this->collision1 = g_interfaces->m_game_resource_service->game_entity_system->get_colision_entity(local_player);
    }
    else
    {
        this->skip_handle1 = -1;
        this->collision1 = -1;
        this->skip_owner_handle1 = -1;
    }

    if (entity)
    {
        this->skip_handle2 = g_interfaces->m_game_resource_service->game_entity_system->get_handle_entity(entity);
        this->skip_owner_handle2 = g_interfaces->m_game_resource_service->game_entity_system->get_handle_entity(g_interfaces->m_game_resource_service->game_entity_system->get_entity(entity->m_owner_entity().get_entry_index()));
        this->collision2 = g_interfaces->m_game_resource_service->game_entity_system->get_colision_entity(entity);
    }
    else
    {
        this->skip_handle2 = -1;
        this->collision2 = -1;
        this->skip_owner_handle2 = -1;
    }

    this->N0000011C = 7;

    this->layer = layer;

    this->flags = 0x49;

    this->null_it3 = 0;
    this->unk1 = 0;
    this->unk2 = 0;
}