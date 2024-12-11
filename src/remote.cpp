#include "remote.hpp"

uint32_t Remote::init(){
    intf.init();
    uint32_t res = net.init(proc.DBC_VERSION, {}, PORT);
    if(res != NET_E_SUCCESS){
        return res;
    }

    return NET_E_SUCCESS;
}

uint32_t Remote::reset(){
    if(intf.needReset() || !intf.isInitialized()){
        intf.reset();
    }
    if(net.needReset() || !net.isInitialized()){
        uint32_t res = net.reset(proc.DBC_VERSION, {}, PORT);
        if(res != NET_E_SUCCESS){
            return res;
        }
    }
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

        net.send(proc.convert(pad).toBytes(proc.DBC_VERSION, proc.DBC_REMOTE_ID));
    }
}