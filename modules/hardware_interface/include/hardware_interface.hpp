#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <xinput.h>
#include "xinput_messages.hpp"

enum GamepadType{
    NONE = 0,
    DINPUT,
    XINPUT
};

class HardwareInterface{
    private:
        bool initialized;
        bool need_reset;
        bool has_update;
        GamepadType type;
        LPDIRECTINPUTDEVICE8 dinputDevice;
        uint8_t dwAxes;
        uint8_t dwButtons;
        int8_t xinput_index;
        GamepadData latest_state;
        GamepadData getState();

        bool initDInput();
        bool initXInput();

    public:
        HardwareInterface();
        ~HardwareInterface();

        void init();
        void reset();
        void close();
        bool isInitialized() const {
            return initialized && type != GamepadType::NONE;
        }
        bool needReset() const {
            return need_reset;
        }

        bool catchUpdate();

        GamepadData poll();
};