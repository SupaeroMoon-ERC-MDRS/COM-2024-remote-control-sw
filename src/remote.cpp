#include "remote.hpp"

uint32_t Remote::init(){
    intf.init();
    uint32_t res = net.init(proc.DBC_VERSION, {}, PORT);
    if(res != NET_E_SUCCESS){
        return res;
    }

    if(intf.isInitialized()){
        std::cout << "HW acquired" << std::endl;
    }
    if(net.isInitialized()){
        std::cout << "Net started" << std::endl;
    }

    return NET_E_SUCCESS;
}

uint32_t Remote::reset(){
    if(intf.needReset() || !intf.isInitialized()){
        intf.reset();
        if(intf.isInitialized()){
            std::cout << "HW reacquired" << std::endl;
        }
    }
    if(net.needReset() || !net.isInitialized()){
        uint32_t res = net.reset(proc.DBC_VERSION, {}, PORT);
        if(res != NET_E_SUCCESS){
            return res;
        }
        if(net.isInitialized()){
            std::cout << "Net restarted" << std::endl;
        }
    }
    return NET_E_SUCCESS;
}

uint32_t Remote::shutdown(){
    intf.shutdown();
    return net.shutdown();
}

uint32_t Remote::run(){
    using namespace std::chrono_literals;
    while(true){
        if(!isInitialized() || needReset()){
            std::this_thread::sleep_for(10ms);
            reset();
            continue;
        }

        net.recv();

        if(proc.isEStop()){
            net.send(proc.e_stop_data.toBytes(proc.DBC_VERSION, proc.DBC_REMOTE_ID));
            std::this_thread::sleep_for(10ms);
            if(!net.hasSubscribers()){
                proc.exitEStop(intf.poll());
                std::cout << "Rover shutdown detected, emergency stop mode exited" << std::endl;
            }
        }
        else{
            GamepadData pad = intf.poll();
            if(!intf.catchUpdate()){
                continue;
            }

            net.send(proc.convert(pad).toBytes(proc.DBC_VERSION, proc.DBC_REMOTE_ID));
        }
    }
}