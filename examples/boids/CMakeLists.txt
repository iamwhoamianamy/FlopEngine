add_library(examples_boids
    boids_window.cpp
    flock.cpp
)

target_link_libraries(examples_boids
    freeglut_static
)