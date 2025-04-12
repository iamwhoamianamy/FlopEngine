#pragma once

#include <ostream>
#include <format>
#include <unordered_map>
#include <chrono>

#include "utils/singleton.hpp"

namespace flp::logger
{

namespace detail
{

// TODO make colors and log_levels compile time

enum class console_color : size_t
{
    white = 0,
    red,
    green,
    blue,
    orange
};

enum class log_level : size_t
{
    fatal = 0,
    error,
    warn,
    info,
    debug,
    trace,
    all
};

class logger_impl
{
private:
    log_level _max_level;
    std::ostream _out;
    std::unordered_map<console_color, std::string> _colors_to_symbols;
    std::unordered_map<log_level, console_color>   _levels_to_colors;
    std::unordered_map<log_level, std::string>     _levels_to_string;

public:
    logger_impl(log_level level = log_level::all);

    void log(std::string_view str);
    void log_with_color(console_color color, std::string_view str);
    void log_with_level(log_level level, std::string_view str);

private:
    void init_color_to_symbol_map();
    void init_level_to_color_map();
    void init_level_to_string_map();

private:
    void set_console_color(console_color color);
    void reset_console_color();
};

template <typename... Args>
inline void log_with_level(log_level level, std::format_string<Args...> format, Args && ...args)
{
    auto str = std::format(format, std::forward<Args>(args)...);
    utils::singleton<detail::logger_impl>::get().log_with_level(level, str);
}

} // namespace detail

static void open(detail::log_level level)
{
    utils::singleton<detail::logger_impl>::get(level);
}

template <typename... Args>
static void log_fatal(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::fatal, format, std::forward<Args>(args)...);
}

template <typename... Args>
static void log_error(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::error, format, std::forward<Args>(args)...);
}

template <typename... Args>
static void log_warn(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::warn, format, std::forward<Args>(args)...);
}

template <typename... Args>
static void log_info(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::info, format, std::forward<Args>(args)...);
}

template <typename... Args>
static void log_debug(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::debug, format, std::forward<Args>(args)...);
}

template <typename... Args>
static void log_trace(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::trace, format, std::forward<Args>(args)...);
}

template <typename... Args>
static void log_all(std::format_string<Args...> format, Args&&... args)
{
    detail::log_with_level(detail::log_level::all, format, std::forward<Args>(args)...);
}

} // namespace flp::logger