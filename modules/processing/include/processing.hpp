#pragma once
#include "can_payload.hpp"

class Processing{
    private:
        CanPayload payload;

    public:
        static const uint16_t DBC_VERSION = 0;
        static const uint16_t DBC_REMOTE_ID = 15;

        Processing(){}
        ~Processing(){}

        CanPayload convert(const GamepadData& pad);
};