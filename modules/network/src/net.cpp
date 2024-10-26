#include "net.hpp"

Net::Net():initialized(false){

}

Net::~Net(){
    close();
}

void Net::init(){
    // set up nonblock socket with bind
    // if everything is ok, then set intialized flag to true else call close()
}

void Net::reset(){
    close();
    init();
}

void Net::close(){
    // close socket
    initialized = false;
}

void Net::recv(){
    // read socket while possible, manage connection list
}

void Net::send(const std::vector<uint8_t> bytes){
    // send bytes to all active connections
}