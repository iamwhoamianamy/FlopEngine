add_executable(example_empty
    empty_window.cpp
    main.cpp
)

target_link_libraries(example_empty
    libs_graphics
    libs_gui
    freeglut_static
    ${LIBSTD_LOCATION}
)