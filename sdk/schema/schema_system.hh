#ifndef VALVE_SCHEMA_SYSTEM
#define VALVE_SCHEMA_SYSTEM

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <optional>
#include "../../../deps/fnv1a/fnv1a.h"

#define SCHEMA(function, type, class, prop) \
__forceinline type& function() \
{ \
    const auto offset = g_schema->get_schema_offset(class, prop);  \
    return *reinterpret_cast< type* >( reinterpret_cast< uintptr_t >( this ) + offset); \
} \

class c_schema_system final
{
public:

    struct schema_offset_t
    {
        uint32_t m_class;
        uint32_t m_prop;
        int32_t m_offset;
    };

    std::unordered_map< uint32_t, std::unordered_map< uint32_t, schema_offset_t > > g_schema_data;

    void init();
    int32_t get_schema_offset(const uint32_t& class_hash, const uint32_t& prop_hash);
};

inline const std::unique_ptr< c_schema_system > g_schema{ new c_schema_system() };

#endif