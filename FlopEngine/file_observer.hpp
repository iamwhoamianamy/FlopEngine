#pragma once
#include <mutex>
#include <filesystem>
#include <chrono>

namespace utils
{

class file_observer
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::unique_lock<std::mutex> _lock;

public:
    file_observer() = delete;
    file_observer(const file_observer&) = delete;
    file_observer(file_observer&&) = delete;
    file_observer operator=(const file_observer&) = delete;
    file_observer operator=(file_observer&&) = delete;

    explicit file_observer(
        std::string_view filename,
        auto observe_interval,
        auto&& on_change,
        bool initial_on_change_call = true);

    void block_if_needed();

private:
    void perform_observing_loop(
        std::string_view filename,
        auto observe_interval,
        auto on_change);

    void call_on_change(auto on_change);
};

file_observer::file_observer(
    std::string_view filename,
    auto observe_interval,
    auto&& on_change,
    bool initial_on_change_call)
{
    _lock = std::unique_lock{_mutex, std::defer_lock};

    if (initial_on_change_call)
    {
        call_on_change(on_change);
    }

    std::jthread([this, filename, observe_interval, on_change]()
        {
            while (true)
            {
                perform_observing_loop(filename, observe_interval, on_change);
            }
        }).detach();
}

inline void file_observer::block_if_needed()
{
    if (_lock.try_lock())
    {
        _lock.unlock();
        _cv.notify_all();
    }
    else
    {
        _cv.wait(_lock);
    }
}

inline void file_observer::perform_observing_loop(
    std::string_view filename,
    auto observe_interval,
    auto on_change)
{
    auto last_modified_time = std::filesystem::last_write_time(filename);
    auto now = std::chrono::file_clock::now();

    auto last_modified_time_point = 
        decltype(last_modified_time)::clock::time_point{last_modified_time};

    auto since_last_modified = 
        std::chrono::duration_cast<decltype(observe_interval)>(now - last_modified_time_point);

    if (since_last_modified < observe_interval)
    {
        call_on_change(on_change);
    }

    std::this_thread::sleep_for(observe_interval);
}

inline void file_observer::call_on_change(auto on_change)
{
    if (_lock.try_lock())
    {
        on_change();

        _lock.unlock();
        _cv.notify_all();
    }
    else
    {
        _cv.wait(_lock);

        _lock.lock();
        on_change();
        _lock.unlock();
        _cv.notify_all();
    }
}

}