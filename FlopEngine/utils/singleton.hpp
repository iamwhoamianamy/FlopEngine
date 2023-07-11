#pragma once

#include <memory>

#define NONCOPYABLE(class)                       \
    class(const class&)     = delete;            \
    class(class&&) noexcept = default;           \
                                                 \
    class& operator=(const class&)     = delete; \
    class& operator=(class&&) noexcept = default; 

namespace utils
{

template <typename T>
struct singleton
{
    NONCOPYABLE(singleton)

    template <typename... Args>
    static T& get(Args&&... args)
    {
        std::call_once(get_once_flag(), [...args = std::forward<Args>(args)]()
            {
                instance_.reset(new T(std::forward<Args>(args)...));
            });

        return *instance_;
    }

private:
    static std::unique_ptr<T> _instance;

    static std::once_flag& get_once_flag()
    {
        static std::once_flag once;
        return once;
    }
};

template <typename T>
std::unique_ptr<T> singleton<T>::_instance{nullptr};

}