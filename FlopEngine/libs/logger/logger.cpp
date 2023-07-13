#include "logger.hpp"
#include <iostream>

using namespace logger::detail;

logger::detail::logger_impl::logger_impl(log_level level)
    : _max_level{level}
    , _out{std::cout.rdbuf()}
{
    init_color_to_symbol_map();
    init_level_to_color_map();
}

void logger::detail::logger_impl::init_color_to_symbol_map()
{
    _colors_to_symbols[console_color::white]  = "\x1b[0m";
    _colors_to_symbols[console_color::red]    = "\x1b[31m";
    _colors_to_symbols[console_color::green]  = "\x1b[32m";
    _colors_to_symbols[console_color::blue]   = "\x1b[34m";
    _colors_to_symbols[console_color::orange] = "\x1b[33m";
}

void logger::detail::logger_impl::init_level_to_color_map()
{
    _levels_to_colors[log_level::fatal] = console_color::red;
    _levels_to_colors[log_level::error] = console_color::red;
    _levels_to_colors[log_level::warn]  = console_color::orange;
    _levels_to_colors[log_level::info]  = console_color::green;
    _levels_to_colors[log_level::debug] = console_color::white;
    _levels_to_colors[log_level::trace] = console_color::white;
    _levels_to_colors[log_level::all]   = console_color::white;
}

void logger_impl::set_console_color(console_color color)
{
    _out << _colors_to_symbols[color];
}

void logger_impl::reset_console_color()
{
    set_console_color(console_color::white);
}
