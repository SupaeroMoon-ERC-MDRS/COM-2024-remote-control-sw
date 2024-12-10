#include "net.hpp"

Net::Net():initialized(false){

}

Net::~Net(){
    close();
}

void Net::init(){
    // set up nonblock socket with bind
    struct sockaddr_in addr;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);    
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); 
    addr.sin_addr.s_addr = INADDR_ANY;

    resuslt = bind(sock, (sockaddr*)&addr, sizeof(addr));

    if(sock < INVALID_SOCKET){
        std::cout << "The socket was not properly created";
        close();
    }else{
        if(result < SOCKET_ERROR){
            std::cout << "The bind was not successful";
            close();
        }else{
            initialized = true;
        }
    }
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
    char buffer[255];
    
    listen(sock, 1);
    while(sock!=INVALID_SOCKET && result!= SOCKET_ERROR){
        connect(sock, (SOCK_ADDR*)&addr, sizeof(addr));
        recv(sock, (SOCKADDR*)&addr, sizeof(addr));
    }
    
    
}

void Net::send(const std::vector<uint8_t> bytes){
    // send bytes to all active connections
}
