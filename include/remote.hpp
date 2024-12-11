#include "hardware_interface.hpp"
#include "processing.hpp"
#include "net.hpp"

#define PORT 12122u

class Remote{
    private:
        bool initialized;
        bool need_reset;

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
            return initialized && intf.isInitialized() && net.isInitialized();
        }
        bool needReset(){
            return need_reset || intf.needReset() || net.needReset();
        }

        uint32_t run();
};