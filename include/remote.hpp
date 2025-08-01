#include <csignal>
#include <functional>
#include <thread>
#include <chrono>
#include "hardware_interface.hpp"
#include "processing.hpp"
#include "remote_net.hpp"

/// The udp port of the remote controller node
#define REMOTE_PORT 12122u

/// @brief A container class to implement interactions between HardwareInterface, Processing and RemoteNet
class Remote{
    private:
        /// @brief The ip of the network interface card RemoteNet is bound to
        std::string ip;

        /// @brief Manages and polls the physical controller
        HardwareInterface intf;

        /// @brief Creates the udpcan message bitfield from the controller state
        Processing proc;

        /// @brief Handles the network related interactions
        RemoteNet net;

    public:
        Remote(){}
        ~Remote(){}

        /// @brief Initializes all submodules
        /// @param ip The ip of the netowork interface card RemoteNet will be bound to
        /// @return NET_E_SUCCESS on success. \n
        /// NET_E_SOCK_FAIL_ASSIGN on socket creation and configuration issues. \n
        /// NET_E_SOCK_FAIL_BIND on socket bind failure \n
        /// NET_E_SOCK_FAIL_FCNTL when broadcast privileges cant be given
        uint32_t init(const std::string ip);

        /// @brief Resets the submodules that need to be reset
        /// @return The return code of Remote::init
        uint32_t reset();

        /// @brief Shuts down all submodules
        /// @return NET_E_SUCCESS on success. \n
        /// NET_E_CANT_CLOSE when the socket cant be closed
        uint32_t shutdown();

        /// @brief Checks whether all submodules are initialized
        /// @return True if all submodules are initialized
        bool isInitialized(){
            return intf.isInitialized() && net.isInitialized();
        }

        /// @brief Checks whether any submodules need to be reset
        /// @return True if a submodule needs to be reset
        bool needReset(){
            return intf.needReset() || net.needReset();
        }

        /// @brief Starts the event loop of the Remote program. Periodically checks controller state and emits udpcan remote control messages. Periodically checks recevied packets for connection requests, ignores data packets.
        /// @return Never returns
        uint32_t run();
};