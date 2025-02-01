#include "remote.hpp"

namespace {
std::function<void(int)> shutdown_handler;
void signal_handler(int signal) { shutdown_handler(signal); }
}

int32_t main(){
    Remote remote;
    remote.init();

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    shutdown_handler = [&remote](int signal) {
        std::cout << "User interrupt detected, shutting down" << std::endl;
        remote.shutdown();
        exit(0);
    };

    remote.run();
    return 0;
}