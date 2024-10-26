#pragma once
#include "xinput_messages.hpp"

class HardwareInterface{
    private:
        bool initialized;
        bool has_update;
        XInputState latest_state;
        XInputState getState();

    public:
        HardwareInterface();
        ~HardwareInterface();

        void init();
        void reset();
        void close();
        bool isInitialized() const {
            return initialized;
        }

        bool catchUpdate();

        XInputGamepad poll();
};