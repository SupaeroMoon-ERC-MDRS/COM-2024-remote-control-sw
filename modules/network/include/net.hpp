#pragma once
#include <stdint.h>
#include <vector>
#include <winsock2.h>

#define PORT std::string("12345")

class Net{
    private:
        bool initialized;
        std::vector<sockaddr_in> connections;
    
    public:
        Net();
        ~Net();

        void init();
        void reset();
        void close();
        bool isInitialized() const {
            return initialized;
        }

        void recv();
        void send(const std::vector<uint8_t> bytes);        
};