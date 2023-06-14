#include "delaunay_triangulator.hpp"

const triangle delaunay_triangulator::encompassing_triangle{
    vector2{-5000, -5000}, 
    vector2{-5000,  5000},
    vector2{ 5000,      0}
};