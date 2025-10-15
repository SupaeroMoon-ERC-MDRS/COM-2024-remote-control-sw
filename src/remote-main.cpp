#include "remote.hpp"

namespace {
/// @brief The function/lambda to call once either SIGINT or SIGTERM is called
std::function<void(int)> shutdown_handler;

/// @brief The function registered to the SIGINT or SIGTERM events. This can not directly be the @ref shutdown_handler because this can not be a lambda
/// @param signal The signal code, given by OS, ignored
void signal_handler(int signal) { shutdown_handler(signal); }
}

int32_t main(int32_t argc, char** argv){
    if(argc != 2){
        std::cout << "Usage: remote-control-sw [OWN INTF IP]" << std::endl;
        return -1;
    }

    std::string ip(argv[1]);

    Remote remote;
    remote.init(ip);

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    shutdown_handler = [&remote](int signal) {
        std::cout << "\r[MAIN] User interrupt detected, shutting down" << std::endl;
        uint32_t res = remote.shutdown();
        std::cout << "[MAIN] Shutdown returned " << res << " exiting" << std::endl;
        exit(0);
    };

    remote.run();
    return 0;
}