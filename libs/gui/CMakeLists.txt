add_library(libs_gui
    button.cpp
    input_param_box.cpp
    input_param_list.cpp
    layout.cpp
    master.cpp
    object.cpp
    split_layout.cpp
)

target_link_libraries(libs_gui
    freeglut_static
)