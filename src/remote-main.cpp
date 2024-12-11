#include "remote.hpp"

namespace {
std::function<void(int)> shutdown_handler;
void signal_handler(int signal) { shutdown_handler(signal); }
}

int32_t main(){
    Remote remote;
    remote.init();

    std::signal(SIGINT, signal_handler);
    std::signal(SIGKILL, signal_handler);
    shutdown_handler = [&remote](int signal) {
        std::cout << "Stop " << std::endl;
        remote.shutdown();
        exit(0);
    };

    remote.run();
    return 0;
}