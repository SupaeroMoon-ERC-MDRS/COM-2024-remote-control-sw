#pragma once
#include "can_payload.hpp"

/// @brief A class responsible for the transformation from controller state to message bitfield as well as the emergency stop mechanisms
class Processing{
    private:
        /// @brief The current message bitfield based on the latest controller state available
        CanPayload payload;

        /// @brief The estop mode flag, if true this module will only emit Processing::e_stop_data regardless of actual controller state
        bool e_stop = false;

    public:
        /// @brief The protocol version
        static const uint16_t DBC_VERSION = 0;

        /// @brief The id of the remote control message
        static const uint16_t DBC_REMOTE_ID = 15;

        /// @brief The message corresponding to an estop signal and neutral movements
        const CanPayload e_stop_data;

        Processing():e_stop_data(CanPayload::eStop()){}
        ~Processing(){}

        /// @brief Updates message bitfield using the controller state given
        /// @param pad The controller state
        /// @param enable When true, enables the estop mechanism
        /// @return The updates message bitfield
        CanPayload convert(const GamepadData& pad, const bool enable);

        /// @brief Checks whether estop mode is active
        /// @return True if estop mode is active
        bool isEStop() const {return e_stop;}

        /// @brief Exits estop mode and continues with controller state given
        /// @param data Controller state 
        void exitEStop(const GamepadData&& data);
};