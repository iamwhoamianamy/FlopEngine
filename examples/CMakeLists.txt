add_subdirectory(attraction)
add_subdirectory(boids)
add_subdirectory(delaunay)
#add_subdirectory(empty)
#add_subdirectory(euler_fluid_simulator)
add_subdirectory(marching)
add_subdirectory(pixel_grid)
add_subdirectory(quadtree)

add_executable(examples
    examples.cpp
)

target_link_libraries(examples
    examples_boids
    examples_attraction
    examples_delaunay
    examples_marching
#    examples_pixel_grid
    examples_quadtree
    libs_graphics
    libs_gui
    libs_logger
    utils
    ${LIBSTD_LOCATION}
)
