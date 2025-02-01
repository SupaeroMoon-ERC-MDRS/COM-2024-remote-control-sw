#pragma once
#include "can_payload.hpp"

class Processing{
    private:
        CanPayload payload;
        bool e_stop = false;

    public:
        static const uint16_t DBC_VERSION = 0;
        static const uint16_t DBC_REMOTE_ID = 15;

        const CanPayload e_stop_data;

        Processing():e_stop_data(CanPayload::eStop()){}
        ~Processing(){}

        CanPayload convert(const GamepadData& pad);
        bool isEStop(){return e_stop;}
};