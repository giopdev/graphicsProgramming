cmake_minimum_required(VERSION 3.10)
project(vulkan_ipc_demo C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

add_executable(tri main.cpp)
target_link_libraries(tri
    ${GLFW_LIBRARIES}
    Vulkan::Vulkan
    dl
    pthread
    X11
    Xxf86vm
    Xrandr
    Xi
)
target_include_directories(tri PRIVATE ${GLFW_INCLUDE_DIRS})

add_library(vk_ipc_override SHARED vk_ipc_override.cpp)
target_link_libraries(vk_ipc_override dl pthread)
target_include_directories(vk_ipc_override PRIVATE .)

add_executable(server vk_ipc_monitor.cpp)
target_link_libraries(server pthread)
target_include_directories(server PRIVATE .)

add_executable(load_game game_loader.c)
target_include_directories(load_game PRIVATE .)

add_executable(server-loader server_loader.c)
target_include_directories(server-loader PRIVATE .)
