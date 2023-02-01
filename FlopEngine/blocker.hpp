#pragma once
#include <mutex>

namespace utils
{

template<typename TestPredicate, typename BlockingFunc>
class blocker_t
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::unique_lock<std::mutex> _lock;

    TestPredicate _when_to_block;
    BlockingFunc _blocking_func;

public:
    blocker_t() = delete;
    blocker_t(const blocker_t&) = delete;
    blocker_t(blocker_t&&) = delete;
    blocker_t operator=(const blocker_t&) = delete;
    blocker_t operator=(blocker_t&&) = delete;

    explicit blocker_t(TestPredicate&& when_to_block, BlockingFunc&& blocking_func);
    void block_on_success();
    void wait_for_unblocking();
};

template<typename TestPredicate, typename BlockingFunc>
inline blocker_t<TestPredicate, BlockingFunc>::blocker_t(
    TestPredicate&& when_to_block, 
    BlockingFunc&& blocking_func) :
    _when_to_block{when_to_block},
    _blocking_func{blocking_func}
{
    _lock = std::unique_lock{_mutex, std::defer_lock};
}

template<typename TestPredicate, typename BlockingFunc>
inline void blocker_t<TestPredicate, BlockingFunc>::block_on_success()
{
    if (_when_to_block())
    {
        if (_lock.try_lock())
        {
            _blocking_func();

            _lock.unlock();
            _cv.notify_all();
        }
        else
        {
            _cv.wait(_lock);
            _lock.lock();

            _blocking_func();

            _lock.unlock();
            _cv.notify_all();
        }
    }
}

template<typename TestPredicate, typename BlockingFunc>
inline void  blocker_t<TestPredicate, BlockingFunc>::wait_for_unblocking()
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