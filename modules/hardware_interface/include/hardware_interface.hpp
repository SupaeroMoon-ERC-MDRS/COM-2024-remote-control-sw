#pragma once

#ifdef _WIN32
#define _WINSOCKAPI_
#include <dinput.h>
#include <dinputd.h>
#include <xinput.h>
#else
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "xinput_messages.hpp"

/// @brief An enum for controller types
enum GamepadType{
    NONE = 0,
    DINPUT,
    XINPUT
};

/// @brief A class to manage and poll the controller
class HardwareInterface{
    private:
        /// Flag to show initialization
        bool initialized;

        /// A flag to show whether reset is needed
        bool need_reset;

        /// A flag to show whether an updated controller state is available
        bool has_update;

        /// The type of controller currently in use
        GamepadType type;

        #ifdef _WIN32
        /// The DInput device handle in windows
        LPDIRECTINPUTDEVICE8 dinputDevice;
        #else
        /// The joystick device file descriptor in linux
        int32_t js_fd;
        #endif

        /// The axis count of the currently managed controller
        uint8_t dwAxes;

        /// The button count of the currently managed controller
        uint8_t dwButtons;

        /// The XInput ID of the currently managed controller in case it is of type @ref GamepadType::XINPUT
        int8_t xinput_index;

        /// The latest state received from the controller
        GamepadData latest_state;

        /// @brief Attempts to acquire a controller of type @ref GamepadType::DINPUT
        /// @return True if a controller was acquired
        bool initDInput();

        /// @brief Attempts to acquire a controller of type @ref GamepadType::XINPUT
        /// @return True if a controller was acquired
        bool initXInput();

        #ifdef _WIN32
        #else
        /// @brief Handles a linux joystick event when an 8 axis controller is in use
        /// @param e The joystick event to process
        void handleAx8(const js_event& e);

        /// @brief Handles a linux joystick event when a 6 axis controller is in use
        /// @param e The joystick event to process
        void handleAx6(const js_event& e);
        #endif

    public:
        HardwareInterface();
        ~HardwareInterface();

        /// @brief Attempts to acquire a controller to use
        void init();

        /// @brief Releases current controller and attempts to acquire a controller to use
        void reset();

        /// @brief Releases current controller
        void shutdown();

        /// @brief Checks whether this module is initialized and has a controller available
        /// @return True if this module is initialized and has a controller available
        bool isInitialized() const {
            return initialized && type != GamepadType::NONE;
        }

        /// @brief Checks whether this module needs to be reset
        /// @return True if this module needs to be reset
        bool needReset() const {
            return need_reset;
        }

        /// @brief Clears the HardwareInterface::has_update flag and returns if it was true
        /// @return True if there is an updated controller state available
        bool catchUpdate();

        /// @brief Polls the controller
        /// @return Returns the latest received controller state
        GamepadData poll();
};