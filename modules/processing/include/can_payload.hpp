#pragma once
#include <vector>
#include "xinput_messages.hpp"

#pragma pack(push,1)
struct CanPayload{
    bool l_top : 1;
    bool l_bottom : 1;
    bool l_right : 1;
    bool l_left : 1;    
    bool r_top : 1;
    bool r_bottom : 1;
    bool r_right : 1;
    bool r_left : 1;
    bool l_shoulder : 1;
    bool r_shoulder  : 1;
    bool e_stop : 1;
    bool reserved : 5;
    uint8_t left_trigger : 8;
    uint8_t right_trigger : 8;
    uint8_t thumb_left_x : 8;
    uint8_t thumb_left_y : 8;
    uint8_t thumb_right_x : 8;
    uint8_t thumb_right_y : 8;

    void update(const GamepadData& pad){
        l_top = pad.l_top;
        l_bottom = pad.l_bottom;
        l_right = pad.l_right;
        l_left = pad.l_left;
        r_top = pad.r_top;
        r_bottom = pad.r_bottom;
        r_right = pad.r_right;
        r_left = pad.r_left;
        l_shoulder = pad.l_shoulder;
        r_shoulder = pad.r_shoulder;
        e_stop = pad.l_left & pad.r_right;
        left_trigger = pad.left_trigger;
        right_trigger = pad.right_trigger;
        thumb_left_x = pad.thumb_left_x;
        thumb_left_y = pad.thumb_left_y;
        thumb_right_x = pad.thumb_right_x;
        thumb_right_y = pad.thumb_right_y;
    }

    std::vector<uint8_t> toBytes(const uint16_t dbc_version, const uint8_t id){
        std::vector<uint8_t> buf(sizeof(CanPayload) + 3, 0);
        *(uint16_t*)(buf.data()) = dbc_version;
        buf[2] = id;
        memcpy(buf.data() + 3, this, sizeof(CanPayload));
        return buf;
    }
};
#pragma pack(pop) 