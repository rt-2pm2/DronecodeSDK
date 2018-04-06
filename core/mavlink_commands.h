#pragma once

#include "mavlink_include.h"
#include "locked_queue.h"
#include <cstdint>
#include <string>
#include <functional>
#include <mutex>

namespace dronecore {

class MAVLinkSystem;

class MAVLinkCommands
{
public:
    explicit MAVLinkCommands(MAVLinkSystem &parent);
    ~MAVLinkCommands();

    enum class Result {
        SUCCESS = 0,
        NO_SYSTEM,
        CONNECTION_ERROR,
        BUSY,
        COMMAND_DENIED,
        TIMEOUT,
        IN_PROGRESS,
        UNKNOWN_ERROR
    };

    typedef std::function<void(Result, float)> command_result_callback_t;

    struct CommandInt {
        uint8_t target_system_id;
        uint8_t target_component_id;
        MAV_FRAME frame = MAV_FRAME_GLOBAL_RELATIVE_ALT;
        uint16_t command;
        bool current = 0;
        bool autocontinue = false;
        // Most of the "Reserved" values in MAVLink spec are NAN.
        struct Params {
            float param1 = NAN;
            float param2 = NAN;
            float param3 = NAN;
            float param4 = NAN;
            int32_t x = 0;
            int32_t y = 0;
            float z = NAN;
        } params;

        // In some cases "Reserved" value could be "0".
        // This utility method can be used in such case.
        static
        void set_as_reserved(Params &params, float reserve_val = NAN)
        {
            params.param1 = reserve_val;
            params.param2 = reserve_val;
            params.param3 = reserve_val;
            params.param4 = reserve_val;
            params.x = 0;
            params.y = 0;
            params.z = reserve_val;
        }
    };

    struct CommandLong {
        uint8_t target_system_id;
        uint8_t target_component_id;
        uint16_t command;
        uint8_t confirmation = 0;
        struct Params {
            float param1 = NAN;
            float param2 = NAN;
            float param3 = NAN;
            float param4 = NAN;
            float param5 = NAN;
            float param6 = NAN;
            float param7 = NAN;
        } params;

        static
        void set_as_reserved(Params &params, float reserve_val = NAN)
        {
            params.param1 = reserve_val;
            params.param2 = reserve_val;
            params.param3 = reserve_val;
            params.param4 = reserve_val;
            params.param5 = reserve_val;
            params.param6 = reserve_val;
            params.param7 = reserve_val;
        }
    };

    Result send_command(CommandInt &cmd);
    Result send_command(CommandLong &cmd);

    void queue_command_async(CommandInt &cmd, command_result_callback_t callback);
    void queue_command_async(CommandLong &cmd, command_result_callback_t callback);

    void do_work();

    static const int DEFAULT_COMPONENT_ID_AUTOPILOT = MAV_COMP_ID_AUTOPILOT1;

    // Non-copyable
    MAVLinkCommands(const MAVLinkCommands &) = delete;
    const MAVLinkCommands &operator=(const MAVLinkCommands &) = delete;

private:
    enum class State {
        NONE,
        WAITING,
        IN_PROGRESS,
        DONE,
        FAILED
    } _state = State::NONE;
    std::mutex _state_mutex {};

    struct Work {
        int retries_to_do = 3;
        double timeout_s = 0.5;
        uint16_t mavlink_command = 0;
        mavlink_message_t mavlink_message {};
        command_result_callback_t callback {};
    };

    void receive_command_ack(mavlink_message_t message);
    void receive_timeout();

    MAVLinkSystem &_parent;
    LockedQueue<Work> _work_queue {};

    void *_timeout_cookie = nullptr;
};

} // namespace dronecore
