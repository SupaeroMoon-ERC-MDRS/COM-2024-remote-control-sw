#pragma once

#ifdef _WIN32
#include <dinput.h>
#include <dinputd.h>
#include <xinput.h>
#else
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#endif

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
        #ifdef _WIN32
        LPDIRECTINPUTDEVICE8 dinputDevice;
        #else
        int32_t js_fd;
        #endif
        uint8_t dwAxes;
        uint8_t dwButtons;
        int8_t xinput_index;
        GamepadData latest_state;
        GamepadData getState();

        bool initDInput();
        bool initXInput();

        #ifdef _WIN32
        #else
        void handleAx8(const js_event& e);
        void handleAx6(const js_event& e);
        #endif

    public:
        HardwareInterface();
        ~HardwareInterface();

        void init();
        void reset();
        void shutdown();
        bool isInitialized() const {
            return initialized && type != GamepadType::NONE;
        }
        bool needReset() const {
            return need_reset;
        }

        bool catchUpdate();

        GamepadData poll();
};