#include "processing.hpp"

CanPayload Processing::convert(const GamepadData& pad){
    payload.update(pad);
    return payload;
}