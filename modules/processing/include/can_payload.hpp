#pragma once
#include <vector>
#include "xinput_messages.hpp"

#pragma pack(push,1)
struct CanPayload{
    uint16_t buttons;
    uint8_t left_trigger;
    uint8_t right_trigger;
    int8_t thumb_left_x;
    int8_t thumb_left_y;
    int8_t thumb_right_x;
    int8_t thumb_right_y;

    void update(const XInputGamepad& pad){
        buttons = pad.buttons;
        left_trigger = pad.left_trigger;
        right_trigger = pad.right_trigger;
        thumb_left_x = pad.thumb_left_x / 256;
        thumb_left_y = pad.thumb_left_y / 256;
        thumb_right_x = pad.thumb_right_x / 256;
        thumb_right_y = pad.thumb_right_y / 256;
    }

    std::vector<uint8_t> toBytes(){
        std::vector<uint8_t> buf(sizeof(CanPayload), 0);
        std::memcpy(buf.data(), this, sizeof(CanPayload));
        return buf;
    }
};
#pragma pack(pop) 