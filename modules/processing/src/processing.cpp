#include "processing.hpp"

CanPayload Processing::convert(const XInputGamepad& pad){
    payload.update(pad);
    return payload;
}