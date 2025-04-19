#pragma once
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "net.hpp"

class RemoteNet{
    private:
        Net net;
    
    public:
        RemoteNet();
        ~RemoteNet();

        inline uint32_t init(const uint16_t dbc_version, const uint16_t port){    
            return net.init(dbc_version, port, NodeType::REMOTE);
        }
        inline uint32_t reset(const uint16_t dbc_version, const uint16_t port){
            return net.reset(dbc_version, port, NodeType::REMOTE);
        }
        inline uint32_t shutdown(){
            return net.shutdown();
        }

        inline bool isInitialized(){
            return net.isInitialized();
        }
        inline bool needReset(){
            return net.needReset();
        }

        inline uint32_t recv(){
            return net.recv();
        }
        inline uint32_t send(const std::vector<uint8_t> bytes){
            return net.send(bytes);
        }

        inline bool hasSubscribers(){
            return net.hasSubscribers();
        }
        
        inline bool hasSubscribers(const NodeType& type){
            return net.hasSubscribers(type);
        }
};