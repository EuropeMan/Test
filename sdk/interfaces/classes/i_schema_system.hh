#ifndef CSGO_SCHEMA_SYSTEM_INTERFACE
#define CSGO_SCHEMA_SYSTEM_INTERFACE

#include"../../../utils/math/utl_vector.hh"
#include"../../../utils/math/utl_hash.hh"
#include"../../../utils/memory/memory.hh"

#include <optional>
#include <string_view>
#include <array>
#include "i_particle_system.h"

#define SCHEMASYSTEM_TYPE_SCOPES_OFFSET 0x188
#define SCHEMASYSTEMTYPESCOPE_OFF1 0x3F8
#define SCHEMASYSTEMTYPESCOPE_OFF2 0x8

using schema_string_t = const char*;

struct schema_metadata_entry_data_t;

class c_schema_system_type_scope;
class c_schema_type;

struct c_schema_class_binding {
    c_schema_class_binding* m_parent;
    const char* m_binary_name;// ex: C_World
    const char* m_module_name;
    const char* m_class_name;
    void* m_class_info_old_synthesized;
    void* m_class_info;
    void* m_this_module_binding_pointer;
    c_schema_type* m_schema_type;
};

class c_schema_type {
public:
    bool get_sizes(int* p_out_size, uint8_t* unk_ptr) {
        return CALL_VIRTUAL(bool, 3, this, p_out_size, unk_ptr);
    }

    bool get_size(int* p_out_size) {
        uint8_t smh = 0;
        return get_sizes(p_out_size, &smh);
    }

public:
    void* m_vtable;                           // 0x0000
    const char* m_name;                       // 0x0008
    c_schema_system_type_scope* m_system_type_scope; // 0x0010
    uint8_t m_type_category;                  // ETypeCategory 0x0018
    uint8_t m_atomic_category;                // EAtomicCategory 0x0019
};

struct schema_class_field_data_t {
    schema_string_t m_name;                   // 0x0000
    c_schema_type* m_schema_type;             // 0x0008
    std::uint32_t m_single_inheritance_offset; // 0x0010
    std::int32_t m_metadata_size;             // 0x0014
    schema_metadata_entry_data_t* m_meta_data; // 0x0018
};

struct schema_class_info_data_t;

struct schema_base_class_info_data_t {
    int32_t m_offset;
    schema_class_info_data_t* m_class;
};

// blyat, ya eto reversil dva dnya, pizdec naxyi

struct schema_class_info_data_t {
private:
    void* m_vtable;                           // 0x0000
public:
    const char* m_name;                       // 0x0008
    char* m_description;                      // 0x0010
    int m_size;                               // 0x0018
    std::int16_t m_field_size;                // 0x001C
    std::int16_t m_static_size;               // 0x001E
    std::int16_t m_metadata_size;             // 0x0020
    std::uint8_t m_align_of;                  // 0x0022
    std::uint8_t m_base_classes_count;        // 0x0023
    char m_pad2[0x4];                         // 0x0024
    schema_class_field_data_t* m_fields;      // 0x0028
    char m_pad3[0x8];                         // 0x0030
    schema_base_class_info_data_t* m_base_classes; // 0x0038
    char m_pad4[0x28];                        // 0x0040

    bool inherits_from(schema_class_info_data_t* p_class_info) {
        if (p_class_info == this && p_class_info != nullptr)
            return true;
        else if (m_base_classes == nullptr || p_class_info == nullptr)
            return false;

        for (int i = 0; i < m_base_classes_count; i++) {
            auto& base_class = m_base_classes[i];
            if (base_class.m_class->inherits_from(p_class_info))
                return true;
        }
        return false;
    }
};

struct schema_enumerator_info_data_t {
    schema_string_t m_name;
    union {
        unsigned char m_value_char;
        unsigned short m_value_short;
        unsigned int m_value_int;
        unsigned long long m_value;
    };
    char m_pad_01[0x10];                      // 0x0010
};

class c_schema_enum_info {
public:
    schema_enumerator_info_data_t m_enum_info_data;
};

class c_schema_enum_binding {
public:
    virtual const char* get_binding_name() = 0;
    virtual c_schema_class_binding* as_class_binding() = 0;
    virtual c_schema_enum_binding* as_enum_binding() = 0;
    virtual const char* get_binary_name() = 0;
    virtual const char* get_project_name() = 0;

public:
    char* m_binding_name_;                    // 0x0008
    char* m_dll_name_;                        // 0x0010
    std::int8_t m_align_;                     // 0x0018
    char m_pad_asd23[0x3];                    // 0x0019
    std::int16_t m_size_;                     // 0x001C
    std::int16_t m_flags_;                    // 0x001E
    schema_enumerator_info_data_t* m_enum_info_;
    char m_pad_01[0x8];                       // 0x0028
    c_schema_system_type_scope* m_type_scope_; // 0x0030
    char m_pad_02[0x8];                       // 0x0038
    std::int32_t m_unk1_;                     // 0x0040
};

class c_schema_system_type_scope {
public:
    void find_declared_class(schema_class_info_data_t** p_return_class, const char* class_name) {
        return CALL_VIRTUAL(void, 2, this, p_return_class, class_name);
    }

    c_schema_type* find_schema_type_by_name(const char* name, std::uintptr_t* p_schema) {
        return CALL_VIRTUAL(c_schema_type*, 4, this, name, p_schema);
    }

    c_schema_type* find_type_declared_class(const char* name) {
        return CALL_VIRTUAL(c_schema_type*, 5, this, name);
    }

    void* m_vtable;                           // 0x0000
    char m_name[256U];                        // 0x0008
    char m_pad_0x200[SCHEMASYSTEMTYPESCOPE_OFF1]; // 0x0108
    c_utl_hash<c_schema_class_binding*, 256, unsigned int> m_hash_classes; // 0x0588
    char m_pad_0x2D80[SCHEMASYSTEMTYPESCOPE_OFF2]; // 0x05C8
    c_utl_hash<c_schema_enum_binding*, 256, unsigned int> m_hash_enumes; // 0x2DD0
};

class i_schema_system {
public:
    VFUNC(c_schema_system_type_scope* (__thiscall*)(void*, const char*, void*), find_type_scope_for_module(const char* module_name), 13, module_name, nullptr);

private:
    char m_pad_001[SCHEMASYSTEM_TYPE_SCOPES_OFFSET]; // 0x0000
public:
    c_utl_vector<c_schema_system_type_scope*> m_vec_type_scopes; // SCHEMASYSTEM_TYPE_SCOPES_OFFSET
};

#endif