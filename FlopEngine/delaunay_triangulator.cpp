#include "delaunay_triangulator.hpp"

const triangle delaunay_triangulator::encompassing_triangle{
    vector2{-100000, -100000}, 
    vector2{-100000,  100000},
    vector2{ 100000,      0}
};