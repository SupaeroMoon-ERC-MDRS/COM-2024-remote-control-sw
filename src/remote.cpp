#include "remote.hpp"

uint32_t Remote::init(){
    intf.init();
    if(!intf.isInitialized()){
        return 2000;
    }
    uint32_t res = net.init(proc.DBC_VERSION, {}, PORT);
    if(res != NET_E_SUCCESS){
        return res;
    }

    initialized = true;
    return NET_E_SUCCESS;
}

uint32_t Remote::reset(){
    intf.reset();
    uint32_t res = net.reset(proc.DBC_VERSION, {}, PORT);
    if(res != NET_E_SUCCESS){
        return res;
    }
    initialized = true;
    return NET_E_SUCCESS;
}

uint32_t Remote::shutdown(){
    intf.shutdown();
    return net.shutdown();
}

uint32_t Remote::run(){
    while(true){
        if(!isInitialized() || needReset()){
            reset();
            continue;
        }

        net.recv();
        GamepadData pad = intf.poll();
        if(!intf.catchUpdate()){
            continue;
        }

        if(net.send(proc.convert(pad).toBytes(proc.DBC_VERSION, proc.DBC_REMOTE_ID)) != NET_E_SUCCESS){
            need_reset = true;
        }
    }
}