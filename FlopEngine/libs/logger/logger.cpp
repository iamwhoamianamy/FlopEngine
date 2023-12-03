#include "logger.hpp"

#include <iostream>

using namespace flp::logger::detail;
using namespace flp::logger;

logger_impl::logger_impl(log_level level)
    : _max_level{level}
    , _out{std::cout.rdbuf()}
{
    init_color_to_symbol_map();
    init_level_to_color_map();
    init_level_to_string_map();
}

void logger_impl::init_color_to_symbol_map()
{
    _colors_to_symbols[console_color::white]  = "\x1b[0m";
    _colors_to_symbols[console_color::red]    = "\x1b[31m";
    _colors_to_symbols[console_color::green]  = "\x1b[32m";
    _colors_to_symbols[console_color::blue]   = "\x1b[34m";
    _colors_to_symbols[console_color::orange] = "\x1b[33m";
}

void logger_impl::init_level_to_color_map()
{
    _levels_to_colors[log_level::fatal] = console_color::red;
    _levels_to_colors[log_level::error] = console_color::red;
    _levels_to_colors[log_level::warn]  = console_color::orange;
    _levels_to_colors[log_level::info]  = console_color::green;
    _levels_to_colors[log_level::debug] = console_color::white;
    _levels_to_colors[log_level::trace] = console_color::white;
    _levels_to_colors[log_level::all]   = console_color::white;
}

void logger_impl::init_level_to_string_map()
{
    _levels_to_string[log_level::fatal] = "fatal";
    _levels_to_string[log_level::error] = "error";
    _levels_to_string[log_level::warn]  = "warn";
    _levels_to_string[log_level::info]  = "info";
    _levels_to_string[log_level::debug] = "debug";
    _levels_to_string[log_level::trace] = "trace";
    _levels_to_string[log_level::all]   = "all";
}

void logger_impl::set_console_color(console_color color)
{
    _out << _colors_to_symbols[color];
}

void logger_impl::reset_console_color()
{
    set_console_color(console_color::white);
}

void logger_impl::log(std::string_view str)
{
    auto now = std::chrono::system_clock::now();

    _out << std::format("{:%H}:{:%M}:{:%S} | ", now, now, now);
    _out << str;
}

void logger_impl::log_with_level(log_level level, std::string_view str)
{
    if (level <= _max_level)
    {
        auto level_str = std::format("{:>5s}", _levels_to_string[level]);
        auto formatted_str = std::format("{} | {}", level_str, str);

        log_with_color(_levels_to_colors[level], formatted_str);
    }
}

void logger_impl::log_with_color(console_color color, std::string_view str)
{
    set_console_color(color);
    log(str);
    reset_console_color();

    _out << "\n";
}
