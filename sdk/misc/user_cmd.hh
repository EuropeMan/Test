#ifndef CSGO_USER_CMD_SYS
#define CSGO_USER_CMD_SYS

#include "../../utils/math/vector.hh"

template <class T>
class c_repeated_ptr_field
{
	std::uint64_t m_unk_field;
	int m_current_size;
	int m_capacity;
	struct inner_container_t
	{
		int m_max;
		T** elements() { return reinterpret_cast<T**>(reinterpret_cast<uintptr_t>(this) + 0x8); }
	}*m_container;
public:
	inline int& size() {return m_current_size;}
	inline int& capacity() {return m_capacity;}
	inline int& max_size() {return m_container->m_max;}
	inline void clear() { m_current_size = 0; }
	inline std::uint64_t unk_field() { return m_unk_field; }

	T* add(T* element) {
		static auto m_rep_field_new = g_sig->get(_("add_to_rep_field_ptr")).as< void* >();
		return reinterpret_cast<T * (__fastcall*)(c_repeated_ptr_field*, T*)>(m_rep_field_new)(this, element);
	}

	inline T*& operator[](int i) { return m_container->elements()[i]; }
	inline operator bool() { return m_container != nullptr; }
};

class c_google_proto {
public:
	void* __vfptr;
	uint64_t m_has_bits;
	uint64_t m_cached_bits;
};

class c_msg_angle : public c_google_proto
{
public:
	vec3_t m_view_angles; //0x0018
};

class c_cmd_vector : public c_google_proto
{
public:
	vec3_t m_view_angles; //0x0018
};

class c_in_button_state_pb : public c_google_proto
{
public:
	std::uint64_t m_value; //0x0018
	std::uint64_t m_changed; //0x0020
	std::uint64_t m_scroll; //0x0028
};
static_assert(sizeof(c_in_button_state_pb) == 0x30);

class c_cs_input_message
{
public:
	std::int32_t m_frame_tick_count; //0x0000
	float m_frame_tick_fraction; //0x0004
	std::int32_t m_player_tick_count; //0x0008
	float m_player_tick_fraction; //0x000C
	vec3_t m_view_angles; //0x0010
	vec3_t m_shoot_position; //0x001C
	std::int32_t m_target_entity_index; //0x0028
	vec3_t m_target_head_pos; //0x002C
	vec3_t m_target_abs_origin; //0x0038
	vec3_t m_target_angle; //0x0044
	std::int32_t m_sv_show_hit_registration; //0x0050
	std::int32_t m_entry_index_max; //0x0054
	std::int32_t m_player_index; //0x0058
	std::uint32_t m_scene_layer; //0x005C
};

class c_csgo_interpolation_info : public c_google_proto
{
public:
	float m_fraction; // 0x18
	int m_src_tick; // 0x1C
	int m_dst_tick; // 0x20
};

class c_input_history_entry : public c_google_proto
{
public:
	c_msg_angle* m_view; // 0x18
	c_cmd_vector* m_shoot_origin_cmd; // 0x20
	c_cmd_vector* m_target_head_origin_cmd; // 0x28
	c_cmd_vector* m_target_abs_origin_cmd; // 0x30
	c_msg_angle* m_target_view_cmd; // 0x38
	c_csgo_interpolation_info* m_cl_interp; // 0x40
	c_csgo_interpolation_info* m_sv_interp0; // 0x48
	c_csgo_interpolation_info* m_sv_interp1; // 0x50
	c_csgo_interpolation_info* m_player_interp; // 0x58
	std::int32_t m_render_tick_count; //0x0060
	float m_render_tick_fraction; //0x0064
	std::int32_t m_player_tick_count; //0x0068
	float m_player_tick_fraction; //0x006C
	int m_frame_number; // 0x70
	int m_target_ent_index; // 0x74

	c_cmd_vector* create_msg_vector()
	{
		m_cached_bits |= 1;

		auto a1 = (DWORD64*)(m_has_bits & 0xFFFFFFFFFFFFFFFC);

		if ((m_has_bits & 1) != 0)
			a1 = (DWORD64*)*a1;

		static auto new_msg_vector = reinterpret_cast<c_cmd_vector * (__fastcall*)(DWORD64*)>(g_sig->get("new_msg_vector").as<void*>());

		return new_msg_vector(a1);
	}

	c_msg_angle* create_msg_angle()
	{
		m_cached_bits |= 1;

		if (m_view)
			return m_view;

		auto a1 = (DWORD64*)(m_has_bits & 0xFFFFFFFFFFFFFFFC);

		if ((m_has_bits & 1) != 0)
			a1 = (DWORD64*)*a1;

		static auto new_msg_angle = reinterpret_cast<c_msg_angle * (__fastcall*)(DWORD64*)>(g_sig->get("create_msg_angle").as<void*>());

		return new_msg_angle(a1);
	}
};

class c_subtick_moves_steps : public c_google_proto
{
public:
	std::uint64_t m_button;
	bool m_pressed;
	float m_when;
	float m_analog_forward_delta;
	float m_aanalog_left_delta;
};
static_assert(sizeof(c_subtick_moves_steps) == 0x30);

class c_base_user_cmd_pb : public c_google_proto
{
public:
	c_repeated_ptr_field<c_subtick_moves_steps> m_subtick_move_steps; //0x0018is 
	std::string* m_move_crc; //0x0028
	c_in_button_state_pb* m_button_state; //0x0038
	c_msg_angle* m_view; //0x0040
	std::int32_t m_command_number; //0x0048
	std::int32_t m_tick_count; //0x004C
	float m_forward_move; //0x0050
	float m_side_move; //0x0054
	float m_up_move; //0x0058
	std::int32_t m_impulse; //0x005C
	std::int32_t m_weapon_select; //0x0060
	std::int32_t m_random_seed; //0x0064
	std::int32_t m_mouse_x; //0x0068
	std::int32_t m_mouse_y; //0x006C
	std::uint32_t m_consumed_server_angle_changes; //0x0070
	std::int32_t m_flags; //0x0074
	std::uint32_t m_entity_handle; //0x0078
private:
	std::uint8_t padding_7c[0x3]{ }; //0x007C
public:

	c_in_button_state_pb* get_button_state()
	{
		m_cached_bits |= 2;
		if (m_button_state)
			return m_button_state;

		auto a1 = (DWORD64*)(m_has_bits & 0xFFFFFFFFFFFFFFFC);

		if ((m_has_bits & 1) != 0)
			a1 = (DWORD64*)*a1;

		static auto result = reinterpret_cast<c_in_button_state_pb * (__fastcall*)(DWORD64*)>(g_sig->get(_("get_button_state_info")).as< void* >())(a1);

		m_button_state = result;
		return result;
	}

	c_msg_angle* get_msg_angle()
	{
		m_cached_bits |= 4;

		if (m_view)
			return m_view;

		auto a1 = (DWORD64*)(m_has_bits & 0xFFFFFFFFFFFFFFFC);

		if ((m_has_bits & 1) != 0)
			a1 = (DWORD64*)*a1;

		static auto result = reinterpret_cast<c_msg_angle * (__fastcall*)(DWORD64*)>(g_sig->get(_("get_view_message")).as< void* >())(a1);
		m_view = result;
		return result;
	}

};
static_assert(sizeof(c_base_user_cmd_pb) == 0x80);

class c_in_button_state
{
	std::uint8_t padding_0[0x8]{ };
public:
	std::uint64_t m_value; //0x0008
	std::uint64_t m_changed; //0x0010
	std::uint64_t m_scroll; //0x0018
};
static_assert(sizeof(c_in_button_state) == 0x20);

class c_user_cmd : public c_google_proto
{
public:
	c_repeated_ptr_field<c_input_history_entry> m_input_history; //0x0018
	c_base_user_cmd_pb* m_base_cmd; //0x0030
	bool m_left_hand; //0x0038
	int32_t attack_start_history_index_3;//0x0034
	int32_t attack_start_history_index_1;//0x0038
	int32_t attack_start_history_index_2;//0x003C
	c_in_button_state m_button_state; //0x0040
	char pad_0068[0x8]; //0x0068
	uint32_t random_seed; //0x0070
	float current_time;//0x0074
	char pad_78[16];//0x0078

	c_input_history_entry* get_input_history_entry(int nIndex)
	{
		if (nIndex >= m_input_history.size())
			return nullptr;

		return m_input_history[nIndex];
	}

	inline bool is_button_state(int button) {
		return (m_button_state.m_value & button)
			 || (m_button_state.m_changed & button);
	}

	void set_cmd_button_state(bool state, int button) {
		this->m_button_state.m_value |= button;
		this->m_button_state.m_changed |= button;
	}

	void remove_cmd_button_state(int button) {
		this->m_button_state.m_value &= ~button; 
		this->m_button_state.m_changed &= ~button;
	}

	void update_attack_history() {
		int idx = this->m_input_history.size() - 1;

		this->attack_start_history_index_1 = idx;
		this->attack_start_history_index_2 = idx;
		this->attack_start_history_index_3 = idx;
	}

	void set_sub_tick_angles(vec3_t& angles)
	{
		auto container = this->m_input_history;
		for (std::int32_t i = 0; i < container.size(); i++)
		{
			c_input_history_entry* tick = container[i];

			if (tick && tick->m_view)
			{
				tick->m_cached_bits |= 1;
				tick->m_view->m_cached_bits |= 7;
				tick->m_view->m_view_angles = angles;
			}
		}
	}

	bool verify_cmd()
	{
		auto btns = m_base_cmd->get_button_state();

		btns->m_cached_bits |= 1;
		btns->m_value = m_button_state.m_value;

		btns = m_base_cmd->get_button_state();
		btns->m_cached_bits |= 2;
		btns->m_changed = m_button_state.m_changed;

		btns = m_base_cmd->get_button_state();
		btns->m_cached_bits |= 4;
		btns->m_scroll = m_button_state.m_scroll;

		c_base_user_cmd_pb* new_base_cmd = new c_base_user_cmd_pb;

		static auto create_new_base_cmd = reinterpret_cast<void* (__fastcall*)(c_base_user_cmd_pb*, int, int)>(g_sig->get(_("create_new_base_cmd")).as< void* >());

		create_new_base_cmd(new_base_cmd, 0, 0);

		auto new_btns = new_base_cmd->get_button_state();

		if (new_btns != m_base_cmd->m_button_state)
		{
			if (new_btns->m_cached_bits & 7)
			{
				new_btns->m_value = 0;
				new_btns->m_changed = 0;
			}

			new_btns->m_cached_bits = 0;

			auto old_bits = m_base_cmd->m_button_state->m_cached_bits;

			if (old_bits & 7)
			{
				if (old_bits & 1)
					new_btns->m_value = m_base_cmd->m_button_state->m_value;

				if (old_bits & 2)
					new_btns->m_changed = m_base_cmd->m_button_state->m_changed;

				if (old_bits & 4)
					new_btns->m_scroll = m_base_cmd->m_button_state->m_scroll;

				new_btns->m_cached_bits = old_bits;
			}

			static auto invalide_button_state = reinterpret_cast<void(__fastcall*)(uint64_t, uint64_t)>(g_sig->get(_("invalide_button_state")).as< void* >());

			if (m_base_cmd->m_button_state->m_has_bits & 1)
				invalide_button_state(new_btns->m_has_bits, (*(DWORD64*)m_base_cmd->get_button_state()->m_has_bits & 0xFFFFFFFFFFFFFFFC) + 8);
		}

		return new_base_cmd->m_has_bits & 1;
	}

	void remove_button_hold(int button) { this->m_button_state.m_value &= ~button; }
};
static_assert(sizeof(c_user_cmd) == 0x88);

#endif