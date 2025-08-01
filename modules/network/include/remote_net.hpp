#pragma once
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "net.hpp"

/// @brief A public API over the required parts of the udpcan-net library
class RemoteNet{
    private:
        /// @brief Represents a node able to communicate using the udpcan protocol
        Net net;
    
    public:
        RemoteNet();
        ~RemoteNet();

        /// @brief Initializes and binds the socket for use. Nonblocking behavior is set with broadcast privileges
        /// @param dbc_version The protocol version to expect
        /// @param ip The IP address to bind to
        /// @param port The port to bind to
        /// @return NET_E_SUCCESS on success. \n
        /// NET_E_SOCK_FAIL_ASSIGN on socket creation and configuration issues. \n
        /// NET_E_SOCK_FAIL_BIND on socket bind failure \n
        /// NET_E_SOCK_FAIL_FCNTL when broadcast privileges cant be given
        inline uint32_t init(const uint16_t dbc_version, const std::string ip, const uint16_t port){    
            return net.init(dbc_version, ip, port, NodeType::REMOTE);
        }

        /// @brief Calls reinitializes the network node with the given parameters.
        /// @return The return code of RemoteNet::init
        inline uint32_t reset(const uint16_t dbc_version, const std::string ip, const uint16_t port){
            return net.reset(dbc_version, ip, port, NodeType::REMOTE);
        }

        /// @brief Sends a packet with HEADER_DECONN to all subscribers and publishers of this node, then closes the socket and clears buffers
        /// @return NET_E_SUCCESS on success. \n
        /// NET_E_CANT_CLOSE when the socket cant be closed
        inline uint32_t shutdown(){
            return net.shutdown();
        }

        /// @brief Checks whether this node is initialized
        /// @return True if this node is initialized
        inline bool isInitialized(){
            return net.isInitialized();
        }

        /// @brief Checks whether this node needs to be reset
        /// @return True if this node needs to be reset
        inline bool needReset(){
            return net.needReset();
        }

        /// @brief Receives all packets, handles control traffic and makes data packets available
        /// @return NET_E_SUCCESS on success. \n
        /// NET_E_SOCK_FAIL_RECV_ERRNO on error. In this case its recommended for the caller to check the errno variable
        inline uint32_t recv(){
            uint32_t res = net.recv();
            std::vector<RecvPacket> _; // ignore, but clear
            net.getPackets(_);
            return res;
        }

        /// @brief Sends a message to all subscribers. If needed the message is fragmented. Thread safety is implemented on this layer
        /// @param bytes The message to be sent
        /// @return NET_E_SUCCESS on success. \n
        /// NET_E_SOCK_FAIL_SEND_ERRNO on error. In this case its recommended for the caller to check the errno variable. \n
        /// NET_E_PARTIAL_MSG when the packet was partially sent. This is likely a temporary issue on OS level, the packet should be resent.
        inline uint32_t send(const std::vector<uint8_t> bytes){
            return net.send(bytes);
        }

        /// @brief Checks whether this node has any subscribers
        /// @return True if this node has any subscribers
        inline bool hasSubscribers(){
            return net.hasSubscribers();
        }
        
        /// @brief Checks whether this node has any subscribers of a given type
        /// @param type The type to look for
        /// @return True if this node has a subscriber of the given type
        inline bool hasSubscribers(const NodeType& type){
            return net.hasSubscribers(type);
        }
};