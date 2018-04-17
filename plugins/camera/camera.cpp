#include "camera.h"
#include "camera_impl.h"

namespace dronecore {

Camera::Camera(System &system) :
    PluginBase(),
    _impl { new CameraImpl(system) }
{
}

Camera::~Camera()
{
}

Camera::Result Camera::take_photo()
{
    return _impl->take_photo();
}

Camera::Result Camera::start_photo_interval(float interval_s)
{
    return _impl->start_photo_interval(interval_s);
}

Camera::Result Camera::stop_photo_interval()
{
    return _impl->stop_photo_interval();
}

Camera::Result Camera::start_video()
{
    return _impl->start_video();
}

Camera::Result Camera::stop_video()
{
    return _impl->stop_video();
}

void Camera::take_photo_async(const result_callback_t &callback)
{
    _impl->take_photo_async(callback);
}

void Camera::start_photo_interval_async(float interval_s, const result_callback_t &callback)
{
    _impl->start_photo_interval_async(interval_s, callback);
}

void Camera::stop_photo_interval_async(const result_callback_t &callback)
{
    _impl->stop_photo_interval_async(callback);
}

void Camera::start_video_async(const result_callback_t &callback)
{
    _impl->start_video_async(callback);
}

Camera::Result Camera::start_video_streaming()
{
    return _impl->start_video_streaming();
}

Camera::Result Camera::stop_video_streaming()
{
    return _impl->stop_video_streaming();
}

void Camera::set_video_stream_settings(const VideoStreamSettings &settings)
{
    return _impl->set_video_stream_settings(settings);
}

Camera::Result Camera::get_video_stream_info(VideoStreamInfo &info)
{
    return  _impl->get_video_stream_info(info);
}

void Camera::stop_video_async(const result_callback_t &callback)
{
    _impl->stop_video_async(callback);
}

void Camera::set_mode_async(Mode mode, const mode_callback_t &callback)
{
    _impl->set_mode_async(mode, callback);
}

void Camera::get_mode_async(const mode_callback_t &callback)
{
    _impl->get_mode_async(callback);
}

void Camera::get_status_async(get_status_callback_t callback)
{
    _impl->get_status_async(callback);
}

void Camera::capture_info_async(capture_info_callback_t callback)
{
    _impl->capture_info_async(callback);
}

void Camera::set_option_key_async(const std::string &setting_key,
                                  const std::string &option_key,
                                  const result_callback_t &callback)
{
    _impl->set_option_key_async(setting_key, option_key, callback);
}

void Camera::get_option_key_async(const std::string &setting_key,
                                  const get_option_callback_t &callback)
{
    _impl->get_option_key_async(setting_key, callback);
}

bool Camera::get_possible_settings(std::vector<std::string> &settings)
{
    return _impl->get_possible_settings(settings);
}

bool Camera::get_possible_options(const std::string &setting_name,
                                  std::vector<std::string> &options)
{
    return _impl->get_possible_options(setting_name, options);
}

const char *Camera::result_str(Result result)
{
    switch (result) {
        case Result::SUCCESS:
            return "Success";
        case Result::IN_PROGRESS:
            return "In progress";
        case Result::BUSY:
            return "Busy";
        case Result::DENIED:
            return "Denied";
        case Result::ERROR:
            return "Error";
        case Result::TIMEOUT:
            return "Timeout";
        case Result::WRONG_ARGUMENT:
            return "Wrong argument";
        case Result::UNKNOWN:
        // FALLTHROUGH
        default:
            return "Unknown";
    }
}

} // namespace dronecore
