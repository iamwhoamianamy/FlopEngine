#pragma once

#include <mutex>
#include <filesystem>
#include <thread>

#include "blocker.hpp"

#include "libs/meta/concepts.hpp"
#include "libs/logger/logger.hpp"

namespace flp::utils
{

struct file_observer
{
public:
    struct settings
    {
        std::filesystem::path filename;
        std::chrono::milliseconds observe_interval{100};
        std::chrono::milliseconds on_error_repeat_interval{1000};
        bool initial_on_change_call = true;
    };

private:
    settings settings_;
    blocker blocker_;
    std::chrono::file_clock::time_point last_modified_and_acked_time_point_{};

public:
    file_observer() = delete;
    file_observer(const file_observer&) = delete;
    file_observer(file_observer&&) = delete;
    file_observer& operator=(const file_observer&) = delete;
    file_observer& operator=(file_observer&&) = delete;


    explicit file_observer(
        settings&& settings,
        std::invocable auto&& on_change);

    void wait_for_unblocking();

private:
    void perform_observing_loop(std::invocable auto&& on_change);
};

file_observer::file_observer(
    settings&& settings,
    std::invocable auto&& on_change)
        : settings_{std::move(settings)}
{
    if (settings_.initial_on_change_call)
    {
        on_change();
    }

    std::jthread([this, on_change]()
        {
            while (true)
            {
                perform_observing_loop(on_change);
            }
        }).detach();
}

inline void file_observer::wait_for_unblocking()
{
    blocker_.wait_for_unblocking();
}

inline void file_observer::perform_observing_loop(std::invocable auto&& on_change)
{
    std::filesystem::file_time_type last_modified_time;

    try
    {
        last_modified_time = std::filesystem::last_write_time(settings_.filename);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        logger::log_trace(
            "file observer: observing loop error: {}, p1: {}, p2: {}",
            e.what(),
            e.path1().generic_string(),
            e.path2().generic_string());

        std::this_thread::sleep_for(settings_.on_error_repeat_interval);
        return;
    }

    if (last_modified_and_acked_time_point_ == std::filesystem::file_time_type{})
    {
        last_modified_and_acked_time_point_ = last_modified_time;
        return;
    }

    if (last_modified_and_acked_time_point_ < last_modified_time)
    {
        blocker_.block(
            [on_change, this, last_modified_time]()
            {
                on_change();
                last_modified_and_acked_time_point_ = last_modified_time;
            }
        );
    }

    std::this_thread::sleep_for(settings_.observe_interval);
}

} // namespace flp::utils