#pragma once
#include <stdint.h>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#endif
#include <iostream>

#define NET_E_SUCCESS 0u

#define NET_E_SOCK_FAIL_ASSIGN 1024u
#define NET_E_SOCK_FAIL_BIND 1025u
#define NET_E_SOCK_FAIL_FCNTL 1026u
#define NET_E_SOCK_FAIL_CLOSE 1027u
#define NET_E_SOCK_FAIL_RECV_ERRNO 1028u
#define NET_E_SOCK_FAIL_SEND_ERRNO 1029u
#define NET_E_PARTIAL_MSG 1030u

struct RecvPacket{
    sockaddr_in addr;
    std::vector<uint8_t> buf;

    RecvPacket(){};
    ~RecvPacket(){};
};

class Net{
    private:
        int32_t socket_fd;
        bool initialized;
        bool need_reset;
        uint16_t expect_dbc_version;
        std::vector<std::pair<uint8_t, uint32_t>> expect_can_ids;
        std::vector<sockaddr_in> connections;
        std::vector<RecvPacket> buf;

        uint32_t readBuf();
        uint32_t readMsg();

        uint32_t sendTo(const std::vector<uint8_t> bytes, const sockaddr_in addr);
    
    public:
        Net();
        ~Net();

        uint32_t init(const uint16_t dbc_version, const std::vector<std::pair<uint8_t, uint32_t>>& expect_can_ids, const uint16_t port);
        uint32_t reset(const uint16_t dbc_version, const std::vector<std::pair<uint8_t, uint32_t>>& expect_can_ids, const uint16_t port);
        uint32_t shutdown();
        bool isInitialized() const {
            return initialized;
        }
        bool needReset() const {
            return need_reset;
        }

        uint32_t recv();
        uint32_t send(const std::vector<uint8_t> bytes);
        bool hasSubscribers() const {return !connections.empty();}
};