#include "processing.hpp"

CanPayload Processing::convert(const GamepadData& pad){
    payload.update(pad);
    e_stop = payload.e_stop;
    return payload;
}