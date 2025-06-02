#include "schema_system.hh"
#include "../interfaces/interfaces.hh"
#include "../../../console.hh"
#include <deque>

void c_schema_system::init()
{
    std::vector<std::string> modules = { _("client.dll"), _("engine2.dll"), _("schemasystem.dll") };

    for (std::string& module : modules) {
        c_schema_system_type_scope* type_scope = g_interfaces->m_schema_system->find_type_scope_for_module(module.c_str());
        if (type_scope == nullptr)
            return;

        int table_size = type_scope->m_hash_classes.count();
        std::uintptr_t* elements = new std::uintptr_t[table_size + 1U];
        int elements_count = type_scope->m_hash_classes.get_elements(0, table_size, elements);

        for (int i = 0; i < elements_count; i++) {
            utl_ts_hash_handle_t element_handle = elements[i];

            if (element_handle == 0)
                continue;

            c_schema_class_binding* class_binding = type_scope->m_hash_classes[element_handle];
            if (class_binding == nullptr)
                continue;

            schema_class_info_data_t* declared_class_info;
            type_scope->find_declared_class(&declared_class_info, class_binding->m_binary_name);

            if (declared_class_info == nullptr || declared_class_info->m_field_size == 0)
                continue;

            auto class_hash = fnv1a::hash(class_binding->m_binary_name);

            for (std::int16_t j = 0; j < declared_class_info->m_field_size; j++) {
                schema_class_field_data_t* field = &declared_class_info->m_fields[j];

                if (!field || !field->m_name)
                    continue;

                auto field_hash = fnv1a::hash(field->m_name);

                g_schema_data[class_hash][field_hash].m_class = class_hash;
                g_schema_data[class_hash][field_hash].m_prop = field_hash;
                g_schema_data[class_hash][field_hash].m_offset = field->m_single_inheritance_offset;
            }
        }
    }
}

int32_t c_schema_system::get_schema_offset(const uint32_t& class_hash, const uint32_t& prop_hash)
{
    return g_schema_data[class_hash][prop_hash].m_offset;
}