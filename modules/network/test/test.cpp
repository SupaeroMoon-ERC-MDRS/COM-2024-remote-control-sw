#include "net.hpp"

int32_t main(){
    Net net;
    net.init();

    uint8_t i = 0;
    while(true){
        if(net.isInitialized()){
            net.recv();
            net.send({i++});
        }
        else{
            net.reset();
        }
    }

    net.close();
    return 0;
}