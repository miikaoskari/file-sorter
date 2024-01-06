cmake_minimum_required(VERSION 3.28)
project(file_sorter C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(file_sorter src/main.c)

target_link_libraries(file_sorter ${GTK4_LIBRARIES})