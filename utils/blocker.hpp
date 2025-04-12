#pragma once

#include <mutex>
#include <condition_variable>

namespace flp::utils
{

class blocker
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::unique_lock<std::mutex> _lock;

public:
    blocker(const blocker&) = delete;
    blocker(blocker&&) = delete;
    blocker& operator=(const blocker&) = delete;
    blocker& operator=(blocker&&) = delete;

    blocker();

    void block(std::invocable auto&& blocking_func);

    void wait_for_unblocking();
};

inline blocker::blocker()
{
    _lock = std::unique_lock{_mutex, std::defer_lock};
}

inline void blocker::block(std::invocable auto&& blocking_func)
{
    if (_lock.try_lock())
    {
        blocking_func();

        _lock.unlock();
        _cv.notify_all();
    }
    else
    {
        _cv.wait(_lock);
        _lock.lock();

        blocking_func();

        _lock.unlock();
        _cv.notify_all();
    }
}

inline void  blocker::wait_for_unblocking()
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

} // namespace flp::utils