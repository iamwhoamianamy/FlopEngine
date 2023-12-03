#include "delaunay_triangulator.hpp"

using namespace flp;

const geo::triangle detail::delaunay_triangulator::encompassing_triangle{
    vector2{-5000, -5000},
    vector2{-5000,  5000},
    vector2{ 5000,      0}
};