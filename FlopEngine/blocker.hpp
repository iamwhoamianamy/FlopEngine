#pragma once
#include <mutex>

namespace utils
{

class blocker_t
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::unique_lock<std::mutex> _lock;

public:
    blocker_t(const blocker_t&) = delete;
    blocker_t(blocker_t&&) = delete;
    blocker_t operator=(const blocker_t&) = delete;
    blocker_t operator=(blocker_t&&) = delete;

    blocker_t();

    void block_on_success(
        std::predicate auto when_to_block,
        std::invocable auto blocking_func);

    void wait_for_unblocking();
};

inline blocker_t::blocker_t()
{
    _lock = std::unique_lock{_mutex, std::defer_lock};
}

inline void blocker_t::block_on_success(
    std::predicate auto when_to_block,
    std::invocable auto blocking_func)
{
    if (when_to_block())
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
}

inline void  blocker_t::wait_for_unblocking()
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

}