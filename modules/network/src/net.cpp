#include "net.hpp"

Net::Net():initialized(false),need_reset(false),expect_dbc_version(0){

}

Net::~Net(){
    shutdown();
}

uint32_t Net::init(const uint16_t dbc_version, const std::vector<std::pair<uint8_t, uint32_t>>& can_ids, const uint16_t port){    
    expect_dbc_version = dbc_version;
    expect_can_ids = can_ids;
    #ifdef _WIN32
    // set up nonblock socket with bind
    struct sockaddr_in addr;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);    
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 
    addr.sin_addr.s_addr = INADDR_ANY;

    resuslt = bind(sock, (sockaddr*)&addr, sizeof(addr));

    if(sock < 0){
        std::cout << "The socket was not properly created";
        close();
    }else{
        if(result < SOCKET_ERROR){
            std::cout << "The bind was not successful";
            shutdown();
        }else{
            initialized = true;
        }
    }
    #else
    socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_fd < 0){
        return NET_E_SOCK_FAIL_ASSIGN;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socket_fd, (const sockaddr *)&addr, sizeof(sockaddr_in)) < 0){
        shutdown();
        return NET_E_SOCK_FAIL_BIND;
    }

    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1){
        shutdown();
        return NET_E_SOCK_FAIL_FCNTL;
    }
    flags = flags | O_NONBLOCK;
    if(fcntl(socket_fd, F_SETFL, flags) != 0){
        shutdown();
        return NET_E_SOCK_FAIL_FCNTL;
    }

    initialized = true;

    return NET_E_SUCCESS;
    #endif
}

uint32_t Net::reset(const uint16_t dbc_version, const std::vector<std::pair<uint8_t, uint32_t>>& can_ids, const uint16_t port){
    shutdown();
    uint32_t res = init(dbc_version, can_ids, port);
    if(isInitialized()){
        need_reset = false;
    }
    return res;
}

uint32_t Net::shutdown(){
    std::vector<uint8_t> buf(3, 2);
    *(uint16_t*)(buf.data()) = expect_dbc_version;
    send(buf);
    #ifdef _WIN32


    #else
    if(close(socket_fd) != 0) return NET_E_SOCK_FAIL_CLOSE;
    #endif
    initialized = false;
    return NET_E_SUCCESS;
}

uint32_t Net::recv(){
    // read socket while possible, manage connection list
    uint32_t res = readBuf();
    if(res != NET_E_SUCCESS){
        return res;
    }
    return readMsg();
}

uint32_t Net::readBuf(){
    #ifdef _WIN32
    #else
    bool stop = false;
    while(!stop){
        sockaddr_in addr;
        socklen_t addr_len = sizeof(sockaddr_in);
        std::vector<uint8_t> buffer(1024u, 0);
        int64_t nread = recvfrom(socket_fd, buffer.data(), 1024, 0, (sockaddr *)&addr, &addr_len);

        if(nread > 0){
            buf.emplace_back().addr = addr;
            buf.back().buf.resize(nread);
            std::copy(buffer.cbegin(), buffer.cbegin() + nread, buf.back().buf.begin());
        }
        else if(errno == EAGAIN){
            stop = true;
        }
        else{
            need_reset = true;
            return NET_E_SOCK_FAIL_RECV_ERRNO;
        }
    }
    return NET_E_SUCCESS;
    #endif
}

uint32_t Net::readMsg(){
    for(const RecvPacket& pack : buf){
        uint32_t pos = 0;
        while(pos + 3u <= pack.buf.size()){
            if(*(uint16_t*)(pack.buf.data() + pos) != expect_dbc_version){
                pos++;
                continue;
            }
            pos += 2;

            uint8_t msg_id = pack.buf[pos];
            pos++;
            if(msg_id <= 2){  // 0 = add me as conn, 1 = ack of conn, 2 = remove me as conn
                pos++;
                std::vector<uint8_t> search(sizeof(sockaddr_in));
                memcpy(search.data(), &pack.addr, sizeof(sockaddr_in));

                auto it = std::find_if(connections.begin(), connections.end(), [&search](sockaddr_in& conn){
                    std::vector<uint8_t> arr(sizeof(sockaddr_in));
                    memcpy(arr.data(), &conn, sizeof(sockaddr_in));
                    return search == arr;
                });
                if(msg_id == 0){
                    if(it == connections.end()){
                        connections.push_back(pack.addr);
                    }
                    sendTo({pack.buf[0], pack.buf[1], 1}, pack.addr);
                }
                else if(it != connections.end() && msg_id == 2){
                    connections.erase(it);
                }
                continue;
            }
        }
    }
    buf.clear();
    return NET_E_SUCCESS;
}

uint32_t Net::send(const std::vector<uint8_t> bytes){
    // send bytes to all active connections
    #ifdef _WIN32
    #else
    for(const sockaddr_in addr : connections){
        uint64_t sent;
        sent = sendto(socket_fd, bytes.data(), bytes.size(), 0, (const sockaddr *)&addr, sizeof(sockaddr_in));

        if(sent == bytes.size()){
            continue;
        }
        else if(sent < 0){
            need_reset = true;  // TODO maybe just mark connection to be removed
            return NET_E_SOCK_FAIL_SEND_ERRNO;
        }
        else{
            return NET_E_PARTIAL_MSG;
        }
    }
    return NET_E_SUCCESS;
    #endif
}

uint32_t Net::sendTo(const std::vector<uint8_t> bytes, const sockaddr_in addr){
    // send bytes to just one active connection
    #ifdef _WIN32
    #else
    uint64_t sent = sendto(socket_fd, bytes.data(), bytes.size(), 0, (const sockaddr *)&addr, sizeof(sockaddr_in));

    if(sent == bytes.size()){
        return NET_E_SUCCESS;
    }
    else if(sent < 0){
        need_reset = true;  // TODO maybe just mark connection to be removed
        return NET_E_SOCK_FAIL_SEND_ERRNO;
    }
    else{
        return NET_E_PARTIAL_MSG;
    }
    #endif
}