#include "remote.hpp"

uint32_t Remote::init(const std::string ip){
    this->ip = ip;
    intf.init();
    uint32_t res = net.init(proc.DBC_VERSION, ip, REMOTE_PORT);
    if(res != NET_E_SUCCESS){
        std::cout << "First time network initialization returned " << res << std::endl;
        return res;
    }

    if(intf.isInitialized()){
        std::cout << "[CONTROLLER] Controller acquired" << std::endl;
    }
    if(net.isInitialized()){
        std::cout << "[NETWORK] Net started at " << ip << ":" << REMOTE_PORT << std::endl;
    }

    if(intf.isInitialized() && net.isInitialized()){
        std::cout << "[MAIN] First time initialization successful, remote control armed" << std::endl;
    }
    else{
        std::cout << "[MAIN] First time controller interface initialization failed " << std::endl;
    }

    return NET_E_SUCCESS;
}

uint32_t Remote::reset(){
    if(intf.needReset() || !intf.isInitialized()){
        intf.reset();
        if(intf.isInitialized()){
            std::cout << "[CONTROLLER] Controller reacquired" << std::endl;
        }
    }
    if(net.needReset() || !net.isInitialized()){
        uint32_t res = net.reset(proc.DBC_VERSION, ip, REMOTE_PORT);
        if(res != NET_E_SUCCESS){
            return res;
        }
        if(net.isInitialized()){
            std::cout << "[NETWORK] Net restarted" << std::endl;
        }
    }

    if(intf.isInitialized() && net.isInitialized()){
        std::cout << "[MAIN] Reset successful, remote control armed" << std::endl;
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
            net.send(proc.e_stop_data.toBytes(proc.DBC_REMOTE_ID));
            std::this_thread::sleep_for(10ms);
            if(!net.hasSubscribers(NodeType::ROVER)){
                proc.exitEStop(intf.poll());
                std::cout << "[PROCESSING] Rover shutdown detected, emergency stop mode exited" << std::endl;
            }
        }
        else{
            GamepadData pad = intf.poll();
            if(!intf.catchUpdate()){
                continue;
            }

            net.send(proc.convert(pad, net.hasSubscribers()).toBytes(proc.DBC_REMOTE_ID));
        }
    }
}