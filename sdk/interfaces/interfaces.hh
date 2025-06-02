#ifndef VALVE_INTERFACES
#define VALVE_INTERFACES

#include <memory>
#include <string_view>
#include "../../utils/math/utl_vector.hh"
#include "classes/i_game_entity_system.hh"
#include "classes/i_schema_system.hh"
#include "classes/i_memalloc.hh"
#include "classes/i_particle_system.h"
#include "classes/i_client_2.hh"
#include "classes/i_csgo_input.hh"
#include "classes/i_engine_trace.hh"
#include "classes/i_engine_client.hh"
#include "classes/i_engine_pvs_manager.hh"
#include "classes/i_event_manager.h"
#include "classes/i_engine_cvar.hh"

#define INTERVAL_PER_TICK 0.015625f
#define TICK_INTERVAL (INTERVAL_PER_TICK)
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))

using create_interface_fn = void* (*) (const char*, int*);
using instantiate_interface_fn = void* (*) ();

class c_interface_reg
{
public:
    c_interface_reg(instantiate_interface_fn fn, const char* name);
    instantiate_interface_fn m_create_fn{ };
    const char* m_name{ };
    c_interface_reg* m_next{ };
};

class c_interfaces final
{
	template < typename T > T get(const char* module_name, std::string_view interface_name);
public:
    void init();
    void dump();

    i_source_2_client* m_client{};

	i_game_resource_service* m_game_resource_service{ };
    i_schema_system* m_schema_system{ };
    i_engine_client* m_engine{ };
    i_cvar* m_cvar{ };

    i_csgo_input* m_input{ };
    i_engine_trace* m_trace{ };
    i_game_event_manager* m_event_manager{ };
    c_engine_pvs_manager* m_pvs_manager{ };
    i_particle_system* m_particle_system{ };
    i_particle_manager* m_particle_manager{ };

    void* m_material_system{};

    i_mem_alloc* m_mem_alloc{ };
};
inline const std::unique_ptr < c_interfaces > g_interfaces{ new c_interfaces() };

#endif