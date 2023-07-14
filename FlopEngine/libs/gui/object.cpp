#include "object.hpp"
#include "master.hpp"

#include "utils/singleton.hpp"

using namespace gui;

std::shared_ptr<object> object::create()
{
    return std::shared_ptr<object>(new object{});
}

std::shared_ptr<object>
object::create(const rectangle& boundary_rectangle)
{
    return std::shared_ptr<object>(new object{boundary_rectangle});
}

object::object()
    : object{rectangle{}}
{

}

object::object(const rectangle& boundary_rectangle)
    : _boundary_rectangle{boundary_rectangle}
{
    init();
}

void gui::object::init()
{
    utils::singleton<master>::get().add_new_object(std::shared_ptr<object>{this});
}

inline object::~object()
{

}

void object::draw()
{

}

const rectangle& object::boundary_rectangle() const
{
    return _boundary_rectangle;
}

bool object::hidden() const
{
    return _hidden;
}

bool object::hovered_over() const
{
    return _hovered_over;
}

void gui::object::set_hovered_over(bool is_hovered_over)
{
    _hovered_over = is_hovered_over;
}