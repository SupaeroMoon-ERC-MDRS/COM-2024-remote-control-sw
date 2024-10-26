#include "hardware_interface.hpp"

HardwareInterface::HardwareInterface():initialized(false), has_update(false){
    
}

HardwareInterface::~HardwareInterface(){
    close();
}

XInputState HardwareInterface::getState(){
    // To figure out
    // If an error is encountered, set the initialized flag to false
    return XInputState();
}

void HardwareInterface::init(){
    // Initialize whatever resources that are required to access the controller
    // if successful, set initialized flag to true
}

void HardwareInterface::reset(){
    close();
    init();
}

void HardwareInterface::close(){
    // Close/Release whatever resources that are required to access the controller
    initialized = false;
}

bool HardwareInterface::catchUpdate(){
    if(has_update){
        has_update = false;
        return true;
    }
    return false;
}

XInputGamepad HardwareInterface::poll(){
    has_update = latest_state.update(getState());
    return latest_state.data;
}