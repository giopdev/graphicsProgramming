cmake_minimum_required(VERSION 3.10)
project(vulkan CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

add_executable(main.bin main.cpp)

target_link_libraries(main.bin
    ${GLFW_LIBRARIES}
    Vulkan::Vulkan
    dl
    pthread
    X11
    Xxf86vm
    Xrandr
    Xi
)

# Include directories for GLFW
target_include_directories(main.bin PRIVATE ${GLFW_INCLUDE_DIRS})
