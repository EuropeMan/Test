#ifndef VALVE_SOURCE_2_CLIENT
#define VALVE_SOURCE_2_CLIENT

#include "../../misc/user_cmd.hh"

class i_source_2_client
{
private:
    VFUNC(void(__thiscall*)(void*, std::int32_t, const vec3_t&), get_view(std::int32_t a2, vec3_t& angle), 74, a2, angle);
    VFUNC(void(__thiscall*)(void*, std::int32_t, const vec3_t&), set_view(std::int32_t splitscreen_slot, vec3_t& angle), 75, splitscreen_slot, angle);
public:
    vec3_t get_view_angles() { vec3_t angle; get_view(0, angle); return angle; }
    void set_view_angles(vec3_t& angle) { set_view(0, angle); }
   // VFUNC(float(__thiscall*)(void*), get_lerp_time(), 166);
};

#endif