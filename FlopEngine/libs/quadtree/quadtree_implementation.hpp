#pragma once
#include "libs/quadtree/quadtree.hpp"

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::quadtree(const quadtree& other)
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::quadtree(quadtree&& other)
{
    move_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
quadtree<Point, Capacity>::quadtree(const rectangle& boundary_rectangle) 
    : _rectangle(boundary_rectangle)
{
}

template<traits::quadtree_point Point, size_t Capacity>
void quadtree<Point, Capacity>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<traits::quadtree_point Point, size_t Capacity>
void quadtree<Point, Capacity>::insert(Point* point)
{
    if(!_rectangle.contains(traits::access<Point>::position(point)))
    {
        return;
    }

    if(_points.size() < Capacity)
    {
        if(!subdivided())
        {
            _points.push_back(point);
        }
        else
        {
            for(auto& child : _children)
            {
                child->insert(point);
            }
        }
    }
    else
    {
        subdivide();
        _points.push_back(point);

        for(auto p : _points)
        {
            for(auto& child : _children)
            {
                child->insert(p);
            }
        }

        _points.clear();
        _points.shrink_to_fit();
    }
}

template<traits::quadtree_point Point, size_t Capacity>
void quadtree<Point, Capacity>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.half_dimensions.x;
    float h = _rectangle.half_dimensions.y;

    vector2 children_half_dimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(new quadtree(rectangle({ x - w / 2, y - h / 2 }, children_half_dimensions)));
    _children.emplace_back(new quadtree(rectangle({ x + w / 2, y - h / 2 }, children_half_dimensions)));
    _children.emplace_back(new quadtree(rectangle({ x + w / 2, y + h / 2 }, children_half_dimensions)));
    _children.emplace_back(new quadtree(rectangle({ x - w / 2, y + h / 2 }, children_half_dimensions)));
}

template<traits::quadtree_point Point, size_t Capacity>
inline std::vector<Point*> quadtree<Point, Capacity>::quarry(const rectangle& range) const
{
    std::vector<Point*> found;
    quarry(range, found);

    return found;
}

template<traits::quadtree_point Point, size_t Capacity>
inline auto quadtree<Point, Capacity>::quarry_as_range(rectangle& range) const
{
    return utils::make_iterator_range(
        const_iterator{const_cast<quadtree*>(this), &range},
        const_iterator::make_end()
    );
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::quarry(const rectangle& range, std::vector<Point*>& found) const
{
    std::stack<const quadtree<Point, Capacity>*> nodes_to_visit;
    nodes_to_visit.push(this);

    while (!nodes_to_visit.empty())
    {
        auto node = nodes_to_visit.top();
        nodes_to_visit.pop();

        if (!node->_rectangle.intersects(range))
            continue;

        for (auto point : node->_points)
            if (range.contains(traits::access<Point>::position(point)))
                found.push_back(point);

        if (node->subdivided())
            for (auto& child : node->_children)
                nodes_to_visit.push(child);
    }
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::clear_data()
{
    if (subdivided())
    {
        for (auto child : _children)
        {
            delete child;
        }

        _children.clear();
    }
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::copy_fields(const quadtree& other)
{
    _rectangle = other._rectangle;
    _points = other._points;
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::move_fields(quadtree&& other)
{
    _rectangle = other._rectangle;
    _points = std::move(other._points);
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::box() const
{
    return this->_rectangle;
}

template<traits::quadtree_point Point, size_t Capacity>
bool quadtree<Point, Capacity>::subdivided() const
{
    return _children.size();
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::children()
{
    return _children;
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::children() const
{
    return _children;
}

template<traits::quadtree_point Point, size_t Capacity>
constexpr auto quadtree<Point, Capacity>::get_capacity() const
{
    return Capacity;
}

template<traits::quadtree_point Point, size_t Capacity>
inline const auto& quadtree<Point, Capacity>::points() const
{
    return _points;
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::operator=(const quadtree<Point, Capacity>& other)
{
    clear_data();
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::operator=(quadtree<Point, Capacity>&& other) noexcept
{
    clear_data();
    move_fields(std::move(other));

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
quadtree<Point, Capacity>::~quadtree()
{
    clear_data();
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator()
    : _cb{std::make_shared<control_block>()}
{
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator(const const_iterator& other)
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator(const_iterator&& other) noexcept
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator(quadtree* root, rectangle* range)
    : _cb{std::make_shared<control_block>(root, range)}
{
    _cb->is_end = false;
    find_first();
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::~const_iterator()
{
}

template<traits::quadtree_point Point, size_t Capacity>
inline Point& quadtree<Point, Capacity>::const_iterator::operator*() const
{
    return *(*(_cb->node_iterator));
}

template<traits::quadtree_point Point, size_t Capacity>
inline Point* quadtree<Point, Capacity>::const_iterator::operator->()
{
    return *(*(_cb->node_iterator));
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator& quadtree<Point, Capacity>::const_iterator::operator++()
{
    find_next();
    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator quadtree<Point, Capacity>::const_iterator::operator++(int)
{
    auto result = const_iterator(*this);

    find_next();

    return result;
}

template<traits::quadtree_point Point, size_t Capacity>
inline bool quadtree<Point, Capacity>::const_iterator::equal(const const_iterator& other) const
{
    return
        _cb->node == other._cb->node &&
        _cb->node_iterator == other._cb->node_iterator ||
        _cb->is_end == true && other._cb->is_end == true;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator& quadtree<Point, Capacity>::const_iterator::operator=(const const_iterator& other)
{
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator& quadtree<Point, Capacity>::const_iterator::operator=(const_iterator&& other) noexcept
{
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator quadtree<Point, Capacity>::const_iterator::make_end()
{
    return const_iterator{};
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::find_first()
{
    _cb->nodes_to_visit.push(_cb->node);

    find_next_impl();
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::find_next()
{
    _cb->node_iterator++;

    if (_cb->node_iterator != _cb->node->points().end())
        return;

    find_next_impl();
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::find_next_impl()
{
    while (!_cb->nodes_to_visit.empty())
    {
        _cb->node = _cb->nodes_to_visit.top();
        _cb->nodes_to_visit.pop();

        if (!_cb->node->_rectangle.intersects(*_cb->range))
            continue;

        auto find_iterator = std::find_if(
            _cb->node->points().begin(), _cb->node->points().end(),
            [&](auto point)
            {
                return _cb->range->contains(traits::access<Point>::position(point));
            });

        if (find_iterator != _cb->node->points().end())
        {
            _cb->node_iterator = find_iterator;
            return;
        }
        else
        {
            if (_cb->node->subdivided())
                for (auto& child : _cb->node->children())
                    _cb->nodes_to_visit.push(child);
        }
    }

    _cb->is_end = true;
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::copy_fields(const const_iterator& other)
{
    _cb = other._cb;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::control_block::control_block()
{

}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::control_block::control_block(quadtree* root, rectangle* range)
    : root{root}
    , node{root}
    , range{range}
{

}

//template<traits::quadtree_point Point, size_t Capacity>
//inline auto begin(const utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range)
//{
//    return range.begin();
//}
//
//template<traits::quadtree_point Point, size_t Capacity>
//inline auto end(const utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range)
//{
//    return range.end();
//}
//
//template<traits::quadtree_point Point, size_t Capacity>
//inline auto begin(utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range)
//{
//    return range.begin();
//}
//
//template<traits::quadtree_point Point, size_t Capacity>
//inline auto end(utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range)
//{
//    return range.end();
//}