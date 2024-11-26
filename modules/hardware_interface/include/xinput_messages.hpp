#pragma once

#ifdef _WIN32
#include <dinput.h>
#include <dinputd.h>
#else
#include <string.h>
#endif

#include <algorithm>
#include <iostream>
#include <vector>
#include <stdint.h>

#pragma pack(push,1)
struct GamepadData{
    bool l_top = 0;
    bool l_bottom = 0;
    bool l_right = 0;
    bool l_left = 0;    
    bool r_top = 0;
    bool r_bottom = 0;
    bool r_right = 0;
    bool r_left = 0;
    bool l_shoulder = 0;
    bool r_shoulder = 0;
    uint8_t left_trigger = 0;
    uint8_t right_trigger = 0;
    uint8_t thumb_left_x = 128;
    uint8_t thumb_left_y = 128;
    uint8_t thumb_right_x = 128;
    uint8_t thumb_right_y = 128;

    bool update(const GamepadData& other){
        std::vector<uint8_t> otherBytes(sizeof(GamepadData));
        std::vector<uint8_t> currentBytes(sizeof(GamepadData));

        #ifdef _WIN32
        std::memcpy(otherBytes.data(), &other, sizeof(GamepadData));
        std::memcpy(currentBytes.data(), this, sizeof(GamepadData));
        #else
        memcpy(otherBytes.data(), &other, sizeof(GamepadData));
        memcpy(currentBytes.data(), this, sizeof(GamepadData));
        #endif

        bool isNew = otherBytes != currentBytes;
        if(!isNew){
            return false;
        }

        #ifdef _WIN32
        std::memcpy(this, &other, sizeof(GamepadData));
        #else
        memcpy(this, &other, sizeof(GamepadData));
        #endif
        return true;
    }

    #ifdef _WIN32
    bool update(const DIJOYSTATE& other, uint8_t dwAxes, uint8_t dwButtons){
        bool isNotNew = true;
        isNotNew &= this->thumb_left_x == other.lX / 0x100u;
        isNotNew &= this->thumb_left_y == other.lY / 0x100u;
        isNotNew &= this->thumb_right_x == other.lZ / 0x100u;
        isNotNew &= this->thumb_right_y == other.lRz / 0x100u;
        if(dwAxes >= 6){
            isNotNew &= this->left_trigger == other.lRx / 0x100u;
            isNotNew &= this->right_trigger == other.lRy / 0x100u;
        }
        else{
            isNotNew &= this->left_trigger == (other.rgbButtons[6] == 0x80u) * 0xFF;
            isNotNew &= this->right_trigger == (other.rgbButtons[7] == 0x80u) * 0xFF;
        }
        isNotNew &= this->l_shoulder == (other.rgbButtons[4] == 0x80u);
        isNotNew &= this->r_shoulder == (other.rgbButtons[5] == 0x80u);
        isNotNew &= this->l_top == (other.rgdwPOV[0] == 0 || other.rgdwPOV[0] == 4500 || other.rgdwPOV[0] == 31500);
        isNotNew &= this->l_bottom == (other.rgdwPOV[0] == 18000 || other.rgdwPOV[0] == 13500 || other.rgdwPOV[0] == 22500);
        isNotNew &= this->l_left == (other.rgdwPOV[0] == 27000 || other.rgdwPOV[0] == 31500 || other.rgdwPOV[0] == 22500);
        isNotNew &= this->l_right == (other.rgdwPOV[0] == 9000 || other.rgdwPOV[0] == 4500 || other.rgdwPOV[0] == 13500);
        isNotNew &= this->r_top == (other.rgbButtons[3] == 0x80u);
        isNotNew &= this->r_bottom == (other.rgbButtons[1] == 0x80u);
        isNotNew &= this->r_left == (other.rgbButtons[0] == 0x80u);
        isNotNew &= this->r_right == (other.rgbButtons[2] == 0x80u);
        if(isNotNew){
            return false;
        }
        this->thumb_left_x = other.lX / 0x100u;
        this->thumb_left_y = other.lY / 0x100u;
        this->thumb_right_x = other.lZ / 0x100u;
        this->thumb_right_y = other.lRz / 0x100u;
        if(dwAxes >= 6){
            this->left_trigger = other.lRx / 0x100u;
            this->right_trigger = other.lRy / 0x100u;
        }
        else{
            this->left_trigger = (other.rgbButtons[6] == 0x80u) * 0xFF;
            this->right_trigger = (other.rgbButtons[7] == 0x80u) * 0xFF;
        }
        this->l_shoulder = (other.rgbButtons[4] == 0x80u);
        this->r_shoulder = (other.rgbButtons[5] == 0x80u);
        this->l_top = (other.rgdwPOV[0] == 0 || other.rgdwPOV[0] == 4500 || other.rgdwPOV[0] == 31500);
        this->l_bottom = (other.rgdwPOV[0] == 18000 || other.rgdwPOV[0] == 13500 || other.rgdwPOV[0] == 22500);
        this->l_left = (other.rgdwPOV[0] == 27000 || other.rgdwPOV[0] == 31500 || other.rgdwPOV[0] == 22500);
        this->l_right = (other.rgdwPOV[0] == 9000 || other.rgdwPOV[0] == 4500 || other.rgdwPOV[0] == 13500);
        this->r_top = (other.rgbButtons[3] == 0x80u);
        this->r_bottom = (other.rgbButtons[1] == 0x80u);
        this->r_left = (other.rgbButtons[0] == 0x80u);
        this->r_right = (other.rgbButtons[2] == 0x80u);
        return true;
    }
    #else
    
    
    #endif
};
#pragma pack(pop) 

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
            #ifdef _WIN32
            std::memcpy(this, &other, sizeof(XInputState));
            #else
            memcpy(this, &other, sizeof(XInputState));
            #endif
            return true;
        }
        return false;
    }
};
#pragma pack(pop) 