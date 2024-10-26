#pragma once
#include <algorithm>
#include <stdint.h>

#pragma pack(push,1)
struct XInputGamepad{
    uint16_t buttons;
    uint8_t left_trigger;
    uint8_t right_trigger;
    int16_t thumb_left_x;
    int16_t thumb_left_y;
    int16_t thumb_right_x;
    int16_t thumb_right_y;
};
#pragma pack(pop) 

#pragma pack(push,1)
struct XInputState{
    uint32_t packet_number;
    XInputGamepad data;

    bool update(const XInputState&& other){
        if(other.packet_number != packet_number){
            std::memcpy(this, &other, sizeof(XInputState));
            return true;
        }
        return false;
    }
};
#pragma pack(pop) 