#include "net.hpp"

Net::Net():initialized(false){

}

Net::~Net(){
    close();
}

void Net::init(){
    // set up nonblock socket with bind
    struct sockaddr_in saServer;

    socket = socket(AF_INET, SOCK_DGRAM, 0);
    saServer.sin_family = AF_INET;
    saServer.sin_port = htons(PORT); 
    saServer.sin_addr.s_addr = inet_addr();

    bind(socket, (SOCKADDR*) &saServer, sizeof(saServer));
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
