#ifndef VALVE_MEMALLOC_SYS
#define VALVE_MEMALLOC_SYS

#include <cstddef>
#include "../../../utils/memory/memory.hh"

class i_mem_alloc
{
public:
	VFUNC(void* (__thiscall*)(void*, std::size_t), alloc(std::size_t size), 1, size);
	VFUNC(void* (__thiscall*)(void*, const void*, std::size_t), re_alloc(const void* memory, std::size_t size), 2, memory, size);
	VFUNC(void(__thiscall*)(void*, const void*), free(const void* memory), 3, memory);
	VFUNC(std::size_t(__thiscall*)(void*, const void*), get_size(const void* memory), 21, memory);
};

#endif