#ifndef CSGO_ENGINE_CLIENT_SYS
#define CSGO_ENGINE_CLIENT_SYS

class c_network_game_client
{
public:
	VFUNC(float(__thiscall*)(void*), get_client_interp_amount(), 62);
};

class i_network_client_service
{
public:
	i_network_client_service() = delete;
	i_network_client_service(i_network_client_service&&) = delete;
	i_network_client_service(const i_network_client_service&) = delete;

	VFUNC(c_network_game_client* (__thiscall*)(void*), get_network_game_client(), 23);
};

class i_net_channel_info
{
public:
	VFUNC(float(__thiscall*)(void*, int), get_latency(int flow), 10, flow);
};

class i_engine_client
{
public:
	VFUNC(int(__thiscall*)(void*), get_max_clients(), 34);
	VFUNC(bool(__thiscall*)(void*), is_in_game(), 35);
	VFUNC(bool(__thiscall*)(void*), is_connected(), 36);
	VFUNC(i_net_channel_info* (__thiscall*)(void*, int), get_net_channel_info(int split_screen_slot), 37, split_screen_slot);
	VFUNC(void(__thiscall*)(void*, int&, int&), get_screen_size(int& width, int& height), 53, width, height);
	VFUNC(const char* (__thiscall*)(void*), get_level_name(), 56);
	VFUNC(const char* (__thiscall*)(void*), get_level_name_short(), 57);
	VFUNC(void(__thiscall*)(void*, int, const char*, int), execute_client_cmd(const char* sz_cmd_string), 43, 0, sz_cmd_string, 0);
private:
	VFUNC(void(__thiscall*)(void*, int&, int), local_idx(int& idx), 47, idx, 0);
public:
	int get_local_player_index()
	{
		int m_index = -1;
		this->local_idx(m_index);
		return m_index + 1;
	}
};

#endif