#include "processing.hpp"

CanPayload Processing::convert(const GamepadData& pad, const bool enable){
    payload.update(pad);
    if(!e_stop && payload.e_stop && enable){
        std::cout << "Emergency stop mode entered, visually verify that the rover stopped safely" << std::endl;
    }
    e_stop = payload.e_stop && enable;
    return payload;
}

void Processing::exitEStop(const GamepadData&& pad){
    e_stop = false;
    payload.update(pad);
}