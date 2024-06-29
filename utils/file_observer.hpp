#pragma once

#include <mutex>
#include <filesystem>
#include <thread>

#include "blocker.hpp"

#include "libs/meta/concepts.hpp"

namespace flp::utils
{

class file_observer
{
private:
    blocker _blocker;

public:
    file_observer() = delete;
    file_observer(const file_observer&) = delete;
    file_observer(file_observer&&) = delete;
    file_observer& operator=(const file_observer&) = delete;
    file_observer& operator=(file_observer&&) = delete;

    explicit file_observer(
        std::string_view filename,
        concepts::duration auto observe_interval,
        std::invocable auto&& on_change,
        bool initial_on_change_call = true);

    void wait_for_unblocking();

private:
    void perform_observing_loop(
        std::string_view filename,
        concepts::duration auto observe_interval,
        std::invocable auto&& on_change);
};

file_observer::file_observer(
    std::string_view filename,
    concepts::duration auto observe_interval,
    std::invocable auto&& on_change,
    bool initial_on_change_call)
{
    if (initial_on_change_call)
    {
        on_change();
    }

    std::jthread([this, filename, observe_interval, on_change]()
        {
            while (true)
            {
                perform_observing_loop(filename, observe_interval, on_change);
            }
        }).detach();
}

inline void file_observer::wait_for_unblocking()
{
    _blocker.wait_for_unblocking();
}

inline void file_observer::perform_observing_loop(
    std::string_view filename,
    concepts::duration auto observe_interval,
    std::invocable auto&& on_change)
{
    std::filesystem::file_time_type last_modified_time;

    try
    {
        last_modified_time = std::filesystem::last_write_time(filename);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        std::this_thread::sleep_for(observe_interval);
        return;
    }

    auto now = std::chrono::file_clock::now();

    auto last_modified_time_point = 
        decltype(last_modified_time)::clock::time_point{last_modified_time};

    auto since_last_modified = 
        std::chrono::duration_cast<decltype(observe_interval)>(now - last_modified_time_point);

    if (since_last_modified < observe_interval)
    {
        _blocker.block(
            [on_change]()
            {
                on_change();
            }
        );
    }

    std::this_thread::sleep_for(observe_interval);
}

} // namespace flp::utils