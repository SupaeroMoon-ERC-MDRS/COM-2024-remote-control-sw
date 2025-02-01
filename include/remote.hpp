#include <csignal>
#include <functional>
#include <thread>
#include <chrono>
#include "hardware_interface.hpp"
#include "processing.hpp"
#include "net.hpp"

#define PORT 12122u

class Remote{
    private:
        HardwareInterface intf;
        Processing proc;
        Net net;

    public:
        Remote(){}
        ~Remote(){}

        uint32_t init();
        uint32_t reset();
        uint32_t shutdown();

        bool isInitialized(){
            return intf.isInitialized() && net.isInitialized();
        }
        bool needReset(){
            return intf.needReset() || net.needReset();
        }

        uint32_t run();
};