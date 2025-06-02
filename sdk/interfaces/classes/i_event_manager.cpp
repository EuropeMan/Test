#include "i_event_manager.h"
#include "../interfaces.hh"

c_player_controller* c_game_event_helper::get_player_controller()
{
    if (!event)
        return nullptr;

    int controller_id{};

    c_buffer buffer;
    buffer.name = "userid";

    event->get_controller_id(controller_id, &buffer);

    if (controller_id == -1)
        return nullptr;

    return g_interfaces->m_game_resource_service->game_entity_system->get<c_player_controller>(controller_id + 1);
}

c_player_controller* c_game_event_helper::get_attacker_controller()
{
    if (!event)
        return nullptr;

    int controller_id{};

    c_buffer buffer;
    buffer.name = "attacker";

    event->get_controller_id(controller_id, &buffer);

    if (controller_id == -1)
        return nullptr;

    return g_interfaces->m_game_resource_service->game_entity_system->get<c_player_controller>(controller_id + 1);
}

int c_game_event_helper::get_damage()
{
    if (!event)
        return -1;

    return event->get_int2("dmg_health", false);
}

int c_game_event_helper::get_health()
{
    if (!event)
        return -1;

    c_buffer buffer;
    buffer.name = "health";

    return event->get_int(&buffer);
}

int c_game_event::get_int2(const char* name, bool unk)
{
    return g_sig->get(_("get_int_for_event")).as<int(__thiscall*)(void*, const char*, bool)>()(this, name, unk);
}