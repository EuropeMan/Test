#include "interfaces.hh"
#include "../../utils/memory/pe.hh"
#include "../../../console.hh"
#include "../../utils/utils.hh"

void c_interfaces::init()
{
    m_game_resource_service = get<i_game_resource_service* const>(_("engine2.dll"), _("GameResourceServiceClientV00"));
    if (!m_game_resource_service)
        g_console->print("[INTERFACE] failed to find->GameResourceService");

    m_schema_system = get<i_schema_system* const>(_("schemasystem.dll"), _("SchemaSystem_"));
    if (!m_schema_system)
        g_console->print("[INTERFACE] failed to find->SchemaSystem");

    m_engine = get<i_engine_client* const>(_("engine2.dll"), _("Source2EngineToClient"));
    if (!m_engine)
        g_console->print("[INTERFACE] failed to find->Source2EngineToClient");
    
    m_cvar = get<i_cvar* const>(_("tier0.dll"), _("VEngineCvar00"));
    if (!m_cvar)
        g_console->print("[INTERFACE] failed to find->VEngineCvar00");

    m_client = get< i_source_2_client* const >(_("client.dll"), _("Source2Client00"));
    if (!m_client)
        g_console->print("[INTERFACE] failed to find->CSGO_client2");

    m_input = g_sig->get(_("get_csgo_input")).abs().get< i_csgo_input* >();
    if (!m_input)
        g_console->print("[INTERFACE] failed to find->CSGO_Input");
    
    m_pvs_manager = g_sig->get(_("get_pvs_manager")).abs().get< c_engine_pvs_manager* >(0);
    if (!m_pvs_manager)
        g_console->print("[INTERFACE] failed to find->CEnginePvsManager");

    m_particle_system = g_sig->get(_("game_particle_manager")).get< i_particle_system*>();
    if (!m_particle_system)
        g_console->print("[INTERFACE] failed to find->m_particle_system");

    m_particle_manager = g_sig->get(_("particle_manager")).get< i_particle_manager*>();
    if (!m_particle_manager)
        g_console->print("[INTERFACE] failed to find->m_particle_manager");

    m_trace = g_sig->get(_("m_engine_trace")).abs(3, 0).get< i_engine_trace* >();
    if (!m_trace)
        g_console->print("[INTERFACE] failed to find->m_trace");

    m_event_manager = g_sig->get(_("game_event_manager")).get< i_game_event_manager* >();
    if (!m_trace)
        g_console->print("[INTERFACE] failed to find->m_event_manager");

    m_mem_alloc = g_utils->m_mem_alloc = *reinterpret_cast<i_mem_alloc**>(g_pe->export_fn(g_pe->get(_("tier0.dll")), _("g_pMemAlloc")));
    if (!m_mem_alloc)
        g_console->print("[INTERFACE] failed to find->g_pMemAlloc");

    m_material_system = g_interfaces->get<void*>("materialsystem2.dll", "VMaterialSystem2_");
}

template < typename T > T c_interfaces::get(const char* module_name, const std::string_view interface_name)
{
    const auto& create_interface = g_pe->export_fn(g_pe->get(module_name), _("CreateInterface"));
    if (!create_interface)
        return T();

    const auto& create_interface_fn = c_address(create_interface);
    if (!create_interface_fn)
        return T();

    const auto interface_reg = create_interface_fn.abs().get< c_interface_reg* >();
    for (auto interface_node = interface_reg; interface_node != nullptr; interface_node = interface_node->m_next)
    {
        const std::string name = interface_node->m_name;
        if (name.starts_with(interface_name) && std::isdigit(name[interface_name.size()]))
        {
            const auto& interface_address = interface_node->m_create_fn();
            if (!interface_address)
                return T();

            g_console->print("[INTERFACE] find interface " + name + "->" + std::to_string((int)(interface_address)));
            return reinterpret_cast<T>(interface_address);
        }
    }
    return T();
}

void c_interfaces::dump()
{
    for (const auto module_name : g_pe->get())
    {
        if (fnv1a::hash(module_name) == fnv1a::hash(_("crashhandler64.dll")))
            continue;

        const auto& create_interface = g_pe->export_fn(g_pe->get(module_name), _("CreateInterface"));
        if (!create_interface)
            continue;

        const auto& create_interface_fn = c_address(create_interface);
        if (!create_interface_fn)
            continue;

        const auto interface_reg = create_interface_fn.abs().get< c_interface_reg* >();
        for (auto interface_node = interface_reg; interface_node != nullptr; interface_node = interface_node->m_next)
        {
            const auto& interface_address = interface_node->m_create_fn();
            if (interface_address)
            {
                const std::string name = interface_node->m_name;
                g_console->print("[DUMPER] " + module_name + "->" + name + "->" + std::to_string((uint8_t)(interface_address)));
            }
        }
    }
}