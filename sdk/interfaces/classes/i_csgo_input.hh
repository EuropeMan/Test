#ifndef CSGO_INPUT_SYS
#define CSGO_INPUT_SYS

#include "../../misc/user_cmd.hh"

class c_subtick_moves
{
public:
    float when; //0x0000
    char pad_0004[4]; //0x0004
    uint64_t button; //0x0008
    bool pressed; //0x0010
    char pad_0011[7]; //0x0011
}; //Size: 0x0018

class c_input_message
{
public:
    int32_t render_tick_count; //0x0000
    float render_fraction; //0x0004
    int32_t player_tick_count; //0x0008
    float player_fraction; //0x000C
    vec3_t angle; //0x0010
    vec3_t position; //0x001C
    char pad_0028[40]; //0x0028
    int32_t N000005D8; //0x0050
    char pad_0054[4]; //0x0054
    int32_t N000005D9; //0x0058
    int32_t N000005E5; //0x005C
}; //Size: 0x0060

class c_input_message_pb
{
public:
    c_input_message history[4];
};

class c_input_messages
{
public:
    int64_t size;
    c_input_message_pb* pb; //0x0000
}; //Size: 0x01E0

class subtick_button
{
public:
    int64_t button; //0x0000
    float forward; //0x0008
    float side; //0x000C
};

class subtick_button_pb
{
public:
    char pad[0x158];
    vec3_t angle;
};

class subtick_buttons
{
public:
    int64_t size;
    subtick_button_pb* pb;
};


class i_csgo_input
{
    void* m_this;
public:
    std::uint8_t padding_1[584]{ }; //0x0008
    c_user_cmd m_commands[150];
    std::uint8_t padding_0[0x88]{ };//0x5200
    bool m_block_shot; //0x5200
    bool m_cam_in_third_person; //0x5201
    std::uint8_t padding_2[6]{ };//0x5202
    vec3_t m_thirdperson_angles; //0x5208
    std::uint8_t padding_3[16]{ };//0x5214
    std::uint32_t m_sequence_number; //0x5224
    std::uint32_t m_last_outgoing_command; //0x5228
    std::uint8_t padding_4[4]{ };//0x522C
    double m_realtime; //0x5230
    std::uint32_t m_buttons_1; //0x5238
    std::uint8_t padding_5[20]{ };//0x523C
    std::uint32_t m_buttons2; //0x5250
    std::uint8_t padding_6[4]{ }; //0x5254
    vec3_t m_vec_move; //0x5258
    int m_mouse_delta_x; //0x5264
    int m_mouse_delta_y; //0x5268
    int m_subtick_count; //0x526C
    c_subtick_moves m_subticks[12]; //0x5270
    vec3_t m_view_angles; //0x5390
    char pad_539C[140]; //0x539C
    subtick_buttons subtick_cache; //0x542C
    char pad_543C[168]; //0x543C
    c_input_messages input_message; //0x54E4
    char pad_54F4[312]; //0x54F4

    c_user_cmd* get_user_cmd()
    {
        return &m_commands[m_sequence_number % 150];
    }
};

#endif