#include "net.hpp"

int32_t main(){
    Net net;
    net.init(0, {}, 12122);

    uint8_t i = 0;
    while(true){
        if(net.isInitialized()){
            net.recv();
            net.send({i++});
        }
        else{
            net.reset(0, {}, 12122);
        }
    }

    net.shutdown();
    return 0;
}