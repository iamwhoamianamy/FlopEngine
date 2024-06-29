add_library(libs_graphics
    base_window.cpp
    drawing.cpp
    glut_functions.cpp
)

target_link_libraries(libs_graphics
    freeglut_static
)