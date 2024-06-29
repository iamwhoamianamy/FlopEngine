CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project(flop_engine)
set(CMAKE_CXX_STANDARD 23)

# freeglut settings
set(FREEGLUT_BUILD_STATIC_LIBS ON)
#set(FREEGLUT_WAYLAND ON)

# includes
include_directories(${CMAKE_SOURCE_DIR})
#include_directories(${CMAKE_SOURCE_DIR}/libs)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/freeglut/include/)

# subdirectories
add_subdirectory(3rdparty)
add_subdirectory(examples)
add_subdirectory(libs)
add_subdirectory(utils)
