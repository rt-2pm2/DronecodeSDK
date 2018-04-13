#pragma once

#include "system.h"
#include "camera.h"
#include "plugin_impl_base.h"
#include "camera_definition.h"
#include "mavlink_system.h"

namespace dronecore {

class CameraImpl : public PluginImplBase
{
public:
    CameraImpl(System &system);
    ~CameraImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    Camera::Result take_photo();

    Camera::Result start_photo_interval(float interval_s);
    Camera::Result stop_photo_interval();

    Camera::Result start_video();
    Camera::Result stop_video();

    void take_photo_async(const Camera::result_callback_t &callback);
    void start_photo_interval_async(float interval_s, const Camera::result_callback_t &callback);
    void stop_photo_interval_async(const Camera::result_callback_t &callback);
    void start_video_async(const Camera::result_callback_t &callback);
    void stop_video_async(const Camera::result_callback_t &callback);

    void set_mode_async(Camera::Mode mode, const Camera::mode_callback_t &callback);
    void get_mode_async(Camera::mode_callback_t callback);

    void capture_info_async(Camera::capture_info_callback_t callback);

    void get_status_async(Camera::get_status_callback_t callback);

    void set_option_key_async(const std::string &setting_key,
                              const std::string &option_key,
                              const Camera::result_callback_t &callback);

    void get_option_key_async(const std::string &setting_key,
                              const Camera::get_option_callback_t &callback);

    bool get_possible_settings(std::map<std::string, std::string> &settings);
    bool get_possible_options(const std::string &setting_name, std::vector<std::string> &options);

    // Non-copyable
    CameraImpl(const CameraImpl &) = delete;
    const CameraImpl &operator=(const CameraImpl &) = delete;

private:
    struct {
        std::mutex mutex {};
        Camera::get_status_callback_t callback {nullptr};
        Camera::Status data {};
        bool received_camera_capture_status {false};
        bool received_storage_information {false};
        void *timeout_cookie {nullptr};
    } _status;

    static constexpr double DEFAULT_TIMEOUT_S = 3.0;

    struct {
        std::mutex mutex {};
        Camera::mode_callback_t callback {nullptr};
        void *timeout_cookie {nullptr};
    } _get_mode;

    struct {
        std::mutex mutex {};
        int sequence;
    } _capture;

    struct {
        std::mutex mutex {};
        Camera::capture_info_callback_t callback {nullptr};
    } _capture_info;

    void receive_set_mode_command_result(MAVLinkCommands::Result command_result,
                                         const Camera::mode_callback_t &callback,
                                         Camera::Mode mode);

    void get_mode_timeout_happened();

    void receive_get_mode_command_result(MAVLinkCommands::Result command_result);

    static Camera::Result camera_result_from_command_result(
        MAVLinkCommands::Result command_result);

    static void receive_command_result(MAVLinkCommands::Result command_result,
                                       const Camera::result_callback_t &callback);

    static bool interval_valid(float interval_s);

    void process_camera_image_captured(const mavlink_message_t &message);
    void process_storage_information(const mavlink_message_t &message);
    void process_camera_capture_status(const mavlink_message_t &message);
    void process_camera_settings(const mavlink_message_t &message);
    void process_camera_information(const mavlink_message_t &message);

    void receive_storage_information_result(MAVLinkCommands::Result result);
    void receive_camera_capture_status_result(MAVLinkCommands::Result result);

    void check_status();

    void status_timeout_happened();

    void load_definition_file(const std::string &uri);
    void receive_int_param(const std::string &name, bool success, int value);
    void receive_float_param(const std::string &name, bool success, float value);

    std::unique_ptr<CameraDefinition> _camera_definition {};
};


} // namespace dronecore
