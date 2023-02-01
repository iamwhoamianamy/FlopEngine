#pragma once
#include <mutex>
#include <filesystem>

#include "blocker.hpp"

namespace utils
{
//
//struct file_observer_test_predicate
//{
//    bool operator()(auto since_last_modified, auto observe_interval)
//    {
//        return since_last_modified < observe_interval;
//    }
//};
//
//struct file_observer_blocking_func
//{
//    void operator()(auto blocking_func)
//    {
//        blocking_func();
//    }
//};

template<typename DurationType>
class file_observer
{
public:
    using blocker_type = blocker_t<std::function<bool()>, std::function<void()>>;

private:
    blocker_type _blocker;
    DurationType _observe_interval;
    DurationType _since_last_modified;

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

    void wait_for_unblocking();

private:
    void perform_observing_loop(
        std::string_view filename,
        auto observe_interval,
        auto on_change);
};

template<typename DurationType>
file_observer<DurationType>::file_observer(
    std::string_view filename,
    auto observe_interval,
    auto&& on_change,
    bool initial_on_change_call) :
    _observe_interval{observe_interval},
    _blocker{
        std::move(std::function<bool()>{[this]() -> bool { return _since_last_modified < _observe_interval; }}),
        std::move(std::function<void()>{[this, &on_change]() { on_change(); }})}
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

template<typename DurationType>
inline void file_observer<DurationType>::wait_for_unblocking()
{
    _blocker.wait_for_unblocking();
}

template<typename DurationType>
inline void file_observer<DurationType>::perform_observing_loop(
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

    _blocker.block_on_success();

    std::this_thread::sleep_for(observe_interval);
}

}