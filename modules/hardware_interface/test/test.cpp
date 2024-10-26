#include <iostream>
#include "hardware_interface.hpp"

int32_t main(){
    HardwareInterface intf;
    intf.init();
    while(true){
        if(intf.isInitialized()){
            XInputGamepad pad = intf.poll();

            if(!intf.catchUpdate()){
                std::cout << "No update" << std::endl;
                continue;
            }

            std::cout << "Buttons: ";
            for(uint32_t i = 0; i < 16; i++){
                bool set = pad.buttons & (uint16_t)std::pow<uint16_t>(2, i);
                std::cout << set << " ";
            }
            std::cout << "\n";

            std::cout << "LTrigger: " << (uint16_t)pad.left_trigger << "\n";
            std::cout << "RTrigger: " << (uint16_t)pad.right_trigger << "\n";
            std::cout << "ThumbLX: " << pad.thumb_left_x << "\n";
            std::cout << "ThumbLY: " << pad.thumb_left_y << "\n";
            std::cout << "ThumbRX: " << pad.thumb_right_x << "\n";
            std::cout << "ThumbRX: " << pad.thumb_right_y << "\n";
            std::cout << std::endl;
        }
        else{
            intf.reset();
        }
    }

    intf.close();
    return 0;
}