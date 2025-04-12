add_executable(example_boids
    boids_window.cpp
    flock.cpp
    main.cpp
)

target_link_libraries(example_boids
    libs_graphics
    libs_gui
    libs_logger
    utils
    freeglut_static
    ${LIBSTD_LOCATION}
)