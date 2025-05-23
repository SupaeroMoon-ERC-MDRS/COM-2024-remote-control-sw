#include "remote.hpp"

namespace {
std::function<void(int)> shutdown_handler;
void signal_handler(int signal) { shutdown_handler(signal); }
}

int32_t main(int32_t argc, char** argv){
    if(argc != 2){
        std::cout << "Usage: remote-control-sw [IP]" << std::endl;
        return -1;
    }

    std::string ip(argv[1]);

    Remote remote;
    remote.init(ip);

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    shutdown_handler = [&remote](int signal) {
        std::cout << "\rUser interrupt detected, shutting down" << std::endl;
        remote.shutdown();
        exit(0);
    };

    remote.run();
    return 0;
}