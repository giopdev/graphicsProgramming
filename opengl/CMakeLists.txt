cmake_minimum_required(VERSION 3.10)
project(main.bin C CXX)

add_executable(main.bin main.cpp glad.c)

target_include_directories(main.bin PRIVATE include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Include for glfw
target_include_directories(main.bin PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(main.bin ${GLFW_LIBRARIES} dl GL)
