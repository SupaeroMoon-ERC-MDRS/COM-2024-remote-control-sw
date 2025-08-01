#pragma once
#include <vector>
#include "xinput_messages.hpp"

#pragma pack(push,1)
/// @brief A bitfield to hold a remote control udpcan message
struct CanPayload{
    /// @brief The top button state in the left button group
    bool l_top : 1;

    /// @brief The bottom button state in the left button group
    bool l_bottom : 1;

    /// @brief The right button state in the left button group
    bool l_right : 1;

    /// @brief The left button state in the left button group
    bool l_left : 1;    

    /// @brief The top button state in the right button group
    bool r_top : 1;

    /// @brief The bottom button state in the right button group
    bool r_bottom : 1;

    /// @brief The right button state in the right button group
    bool r_right : 1;

    /// @brief The left button state in the right button group
    bool r_left : 1;

    /// @brief The left shoulder button state
    bool l_shoulder : 1;

    /// @brief The right shoulder button state
    bool r_shoulder  : 1;

    /// @brief The emergency stop flag
    bool e_stop : 1;

    /// @brief Unused, padding for byte alignment
    bool reserved : 5;

    /// @brief The left trigger state, usually found beside the left shoulder button. On 6 axis controllers this is a button, else it is an analog
    uint8_t left_trigger : 8;

    /// @brief The right trigger state, usually found beside the right shoulder button. On 6 axis controllers this is a button, else it is an analog
    uint8_t right_trigger : 8;

    /// @brief Position of the left stick in the x axis, such that the maximum left position corresponds to 0, maximum right to 255
    uint8_t thumb_left_x : 8;

    /// @brief Position of the left stick in the y axis, such that the maximum up position corresponds to 0, maximum down to 255
    uint8_t thumb_left_y : 8;

    /// @brief Position of the right stick in the x axis, such that the maximum left position corresponds to 0, maximum right to 255
    uint8_t thumb_right_x : 8;

    /// @brief Position of the right stick in the y axis, such that the maximum up position corresponds to 0, maximum down to 255
    uint8_t thumb_right_y : 8;

    /// @brief Generates the message corresponding to an estop signal
    /// @return The message corresponding to an estop signal and neutral movements
    static CanPayload eStop(){
        CanPayload ret;
        ret.l_top = false;
        ret.l_bottom = false;
        ret.l_right = false;
        ret.l_left = true;
        ret.r_top = false;
        ret.r_bottom = false;
        ret.r_right = true;
        ret.r_left = false;
        ret.l_shoulder = false;
        ret.r_shoulder = false;
        ret.e_stop = true;
        ret.left_trigger = 0u;
        ret.right_trigger = 0u;
        ret.thumb_left_x = 128u;
        ret.thumb_left_y = 128u;
        ret.thumb_right_x = 128u;
        ret.thumb_right_y = 128u;
        return ret;
    }

    /// @brief Updates the message bitfield from a controller state object
    /// @param pad The controller state object
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

    /// @brief Marshals the message bitfield object to a byte buffer
    /// @param id The id of this message
    /// @return The bytes corresponding to this message
    std::vector<uint8_t> toBytes(const uint8_t id) const {
        std::vector<uint8_t> buf(sizeof(CanPayload) + 1, 0);
        buf[0] = id;
        memcpy(buf.data() + 1, this, sizeof(CanPayload));
        return buf;
    }
};
#pragma pack(pop) 