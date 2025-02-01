#include "processing.hpp"

CanPayload Processing::convert(const GamepadData& pad){
    payload.update(pad);
    e_stop = payload.e_stop;
    if(e_stop){
        std::cout << "Emergency stop mode entered, visually verify that the rover stopped safely" << std::endl;
    }
    return payload;
}

void Processing::exitEStop(const GamepadData&& pad){
    e_stop = false;
    payload.update(pad);
}