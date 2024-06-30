add_library(examples_gui
    gui_window.cpp
)

target_link_libraries(examples_gui
    libs_gui
    libs_graphics
)