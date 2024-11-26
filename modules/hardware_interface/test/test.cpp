#include <iostream>
#include <thread>
#include <chrono>
#include "hardware_interface.hpp"

int32_t main(){
    using namespace std::chrono_literals;
    HardwareInterface intf;
    intf.init();
    while(true){
        if(intf.isInitialized() && !intf.needReset()){
            GamepadData pad = intf.poll();

            if(!intf.catchUpdate()){
                //std::cout << "No update" << std::endl;
                continue;
            }

            std::cout << "====================\n";
            std::cout << "LB: " << pad.l_bottom << "\n";
            std::cout << "LT: " << pad.l_top << "\n";
            std::cout << "LR: " << pad.l_right << "\n";
            std::cout << "LL: " << pad.l_left << "\n";
            std::cout << "RB: " << pad.r_bottom << "\n";
            std::cout << "RT: " << pad.r_top << "\n";
            std::cout << "RR: " << pad.r_right << "\n";
            std::cout << "RL: " << pad.r_left << "\n";
            std::cout << "LS: " << pad.l_shoulder << "\n";
            std::cout << "RS: " << pad.r_shoulder << "\n";
            std::cout << "LTrigger: " << (uint16_t)pad.left_trigger << "\n"; // 0 not pressed 255 full pressed
            std::cout << "RTrigger: " << (uint16_t)pad.right_trigger << "\n"; // 0 not pressed 255 full pressed
            std::cout << "ThumbLX: " << (uint16_t)pad.thumb_left_x << "\n"; // left < right
            std::cout << "ThumbLY: " << (uint16_t)pad.thumb_left_y << "\n"; // top < bottom
            std::cout << "ThumbRX: " << (uint16_t)pad.thumb_right_x << "\n"; // left < right
            std::cout << "ThumbRY: " << (uint16_t)pad.thumb_right_y; // top < bottom
            std::cout << std::endl;

            if(pad.r_right && pad.l_left){
                intf.reset();
                std::cout << "reset" << std::endl;
            }
        }
        else{
            intf.reset();
        }
        std::this_thread::sleep_for(100ms);
    }

    intf.shutdown();
    return 0;
}