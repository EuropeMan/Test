#ifndef CSGO_ENGINE_PVS_MANAGER
#define CSGO_ENGINE_PVS_MANAGER

#include "../../../utils/memory/memory.hh"

class c_engine_pvs_manager
{
public:
    VFUNC(void(__thiscall*)(void*, bool), set_pvs(bool state), 7, state);
};

#endif