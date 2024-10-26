#pragma once
#include "can_payload.hpp"

class Processing{
    private:
        CanPayload payload;

    public:
        static const uint16_t DBC_VERSION = 0;

        Processing(){}
        ~Processing(){}

        CanPayload convert(const XInputGamepad& pad);
};