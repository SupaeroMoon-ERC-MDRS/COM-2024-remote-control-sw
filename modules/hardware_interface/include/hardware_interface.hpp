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

/**
 * @enum GamepadType
 * @brief Enumeration representing different types of gamepads.
 *
 * This enum is used to specify the type of gamepad that is being used.
 * It can represent a non-existent gamepad, a DirectInput-based gamepad, 
 * or an XInput-based gamepad.
 */
enum GamepadType {
    /**
     * @brief No gamepad connected or detected.
     *
     * This value represents a situation where no gamepad is connected 
     * or detected by the system.
     */
    NONE = 0,    ///< No gamepad connected or detected.
    
    /**
     * @brief A gamepad using DirectInput.
     *
     * DirectInput (DINPUT) is a legacy API provided by Microsoft as part of the
     * DirectX suite. It allows for interaction with a wide range of input devices,
     * including older or custom gamepads, joysticks, racing wheels, and more.
     *
     * Key Features of DirectInput:
     * - Supports many different types of controllers (including legacy hardware).
     * - Allows fine control over button presses, axis movement, and pressure sensitivity.
     * - Works with older devices that do not support newer APIs like XInput.
     * 
     * Limitations:
     * - Requires custom handling for different devices.
     * - Does not provide standardized support for modern controllers.
     * - Limited vibration feedback support.
     */
    DINPUT,      ///< DirectInput-based gamepad.
    
    /**
     * @brief A gamepad using XInput.
     *
     * XInput is a modern API designed specifically for Xbox controllers, starting with the
     * Xbox 360 and continuing to the Xbox One and Xbox Series controllers.
     *
     * Key Features of XInput:
     * - Provides standardized support for Xbox controllers.
     * - Built-in vibration (force feedback) support.
     * - Simple, plug-and-play setup for Xbox controllers on Windows.
     * - Easy-to-use API, ideal for gamepad handling.
     * 
     * Limitations:
     * - Supports only Xbox controllers.
     * - Does not support other types of controllers or input devices.
     * - Limited customizability compared to DirectInput.
     */
    XINPUT       ///< XInput-based gamepad.
};


class HardwareInterface{
    private:
        bool initialized;
        bool need_reset;
        bool has_update;
        GamepadType type;
        #ifdef _WIN32
        LPDIRECTINPUTDEVICE8 dinputDevice;
        #else
        int32_t js_fd;
        #endif
        uint8_t dwAxes;
        uint8_t dwButtons;
        int8_t xinput_index;
        GamepadData latest_state;
        GamepadData getState();

        bool initDInput();
        bool initXInput();

        #ifdef _WIN32
        #else
        void handleAx8(const js_event& e);
        void handleAx6(const js_event& e);
        #endif

    public:
        HardwareInterface();
        ~HardwareInterface();

        void init();
        void reset();
        void shutdown();
        bool isInitialized() const {
            return initialized && type != GamepadType::NONE;
        }
        bool needReset() const {
            return need_reset;
        }

        bool catchUpdate();

        GamepadData poll();
};