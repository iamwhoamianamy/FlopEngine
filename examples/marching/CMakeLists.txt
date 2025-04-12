add_executable(example_marching
    marching_window.cpp
    main.cpp
)

target_link_libraries(example_marching
    libs_graphics
    libs_gui
    libs_logger
    utils
    freeglut_static
    ${LIBSTD_LOCATION}
)